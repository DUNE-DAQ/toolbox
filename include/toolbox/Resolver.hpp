#ifndef TOOLBOX_INCLUDE_TOOLBOX_RESOLVER_HPP_
#define TOOLBOX_INCLUDE_TOOLBOX_RESOLVER_HPP_

/**
 *
 * @file Resolver.hpp DNS Resolver methods
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "ers/Issue.hpp"
#include "logging/Logging.hpp"

#include <arpa/nameser.h>
#include <netdb.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/types.h>

#include <string>
#include <vector>

namespace dunedaq {

ERS_DECLARE_ISSUE(toolbox,
                  ServiceNotFound,
                  "The service " << service << " was not found in DNS",
                  ((std::string)service))
ERS_DECLARE_ISSUE(toolbox,
                  NameNotFound,
                  "The hostname " << name << " could not be resolved: " << error,
                  ((std::string)name)((std::string)error))

namespace toolbox {
std::vector<std::string>
GetServiceAddresses(std::string service_name, std::string hostname)
{
  std::vector<std::string> output;
  unsigned char query_buffer[1024];

  // Check if we're given a "bare" service name, convert to DNS service name, assuming TCP
  if (std::count(service_name.begin(), service_name.end(), '.') == 0) {
    service_name = "_" + service_name + "._tcp";

    if (!hostname.empty()) {
      service_name += "." + hostname;
    }
  }

  auto response = res_search(service_name.c_str(), C_IN, ns_t_srv, query_buffer, sizeof(query_buffer));
  if (response < 0) {
    ers::error(ServiceNotFound(ERS_HERE, service_name));
    return output;
  }

  ns_msg nsMsg;
  ns_initparse(query_buffer, response, &nsMsg);

  for (int x = 0; x < ns_msg_count(nsMsg, ns_s_an); x++) {
    ns_rr rr;
    ns_parserr(&nsMsg, ns_s_an, x, &rr);

    char name[1024];
    dn_expand(ns_msg_base(nsMsg), ns_msg_end(nsMsg), ns_rr_rdata(rr) + 6, name, sizeof(name));

    auto port = ntohs(*((unsigned short*)ns_rr_rdata(rr) + 2)); // NOLINT(runtime/int)

    struct addrinfo* result;
    auto s = getaddrinfo(name, nullptr, nullptr, &result);

    if (s != 0) {
      ers::error(NameNotFound(ERS_HERE, name, std::string(gai_strerror(s))));
      continue;
    }

    for (auto rp = result; rp != nullptr; rp = rp->ai_next) {
      char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
      getnameinfo(rp->ai_addr, rp->ai_addrlen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV);
      output.push_back(std::string(hbuf) + ":" + std::to_string(port));
    }

    freeaddrinfo(result);
  }
  return output;
}
} // namespace toolbox
} // namespace dunedaq

#endif // TOOLBOX_INCLUDE_TOOLBOX_RESOLVER_HPP_
