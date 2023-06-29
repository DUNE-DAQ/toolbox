/**
 * @file TimestampEstimatorSystem.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "utilities/TimestampEstimatorSystem.hpp"

#include "logging/Logging.hpp"

#include <chrono>

namespace dunedaq {
namespace utilities {

TimestampEstimatorSystem::TimestampEstimatorSystem(uint64_t clock_frequency_hz) // NOLINT(build/unsigned)
  : m_clock_frequency_hz(clock_frequency_hz)
{
  TLOG_DEBUG(0) << "Clock frequency is " << m_clock_frequency_hz
                << " clock_frequency_hz/1000000.=" << (m_clock_frequency_hz / 1000000.);
}

daqdataformats::timestamp_t
TimestampEstimatorSystem::get_timestamp_estimate() const
{
  auto now = std::chrono::system_clock::now().time_since_epoch();
  auto now_us = std::chrono::duration_cast<std::chrono::microseconds>(now);
  return (m_clock_frequency_hz / 1000000.) * now_us.count();
}

} // namespace utilities
} // namespace dunedaq