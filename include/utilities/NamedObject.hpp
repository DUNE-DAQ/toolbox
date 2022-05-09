/**
 * @file Named.hpp Named class interface
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef UTILITIES_INCLUDE_UTILITIES_NAMEDOBJECT_HPP_
#define UTILITIES_INCLUDE_UTILITIES_NAMEDOBJECT_HPP_

#include <string>

namespace dunedaq::utilities {
/**
 * @brief A Named is a DAQ object (Queue or DAQModule) which has an
 * instance name
 */
class Named
{
public:
  /**
   * @brief Named Constructor
   * @param name Name of this object
   */
  Named() = default;                       ///< Named is default-constructible
  Named(Named const&) = delete;            ///< Named is not copy-constructible
  Named(Named&&) = default;                ///< Named is move-constructible
  Named& operator=(Named const&) = delete; ///< Named is not copy-assignable
  Named& operator=(Named&&) = default;     ///< Named is move-assignable
  virtual ~Named() = default;              ///< Default virtual destructor

  /**
   * @brief Get the name of this Named
   * @return The name of this Named
   */
  virtual const std::string& get_name() const = 0;
};

/**
 * @brief Implements the Named interface
 */
class NamedObject : public Named
{
public:
  /**
   * @brief NamedObject Constructor
   * @param name Name of this object
   */
  explicit NamedObject(const std::string& name)
    : m_name(name)
  {}

  NamedObject(NamedObject const&) = delete;            ///< NamedObject is not copy-constructible
  NamedObject(NamedObject&&) = default;                ///< NamedObject is move-constructible
  NamedObject& operator=(NamedObject const&) = delete; ///< NamedObject is not copy-assignable
  NamedObject& operator=(NamedObject&&) = default;     ///< NamedObject is move-assignable
  virtual ~NamedObject() = default;                    ///< Default virtual destructor

  /**
   * @brief Get the name of this NamedObejct
   * @return The name of this NamedObject
   */
  const std::string& get_name() const final { return m_name; }

private:
  std::string m_name;
};

} // namespace dunedaq::utilities
#endif // UTILITIES_INCLUDE_UTILITIES_NAMEDOBJECT_HPP_
