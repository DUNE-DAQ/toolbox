/**
 * @file TimestampEstimatorBase.cpp
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "utilities/TimestampEstimatorBase.hpp"

#include <thread>

namespace dunedaq {
namespace utilities {
TimestampEstimatorBase::WaitStatus
TimestampEstimatorBase::wait_for_valid_timestamp(std::atomic<bool>& continue_flag)
{
  while (continue_flag.load() && get_timestamp_estimate() == std::numeric_limits<uint64_t>::max()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return continue_flag.load() ? TimestampEstimatorBase::kFinished : TimestampEstimatorBase::kInterrupted;
}

TimestampEstimatorBase::WaitStatus
TimestampEstimatorBase::wait_for_timestamp(uint64_t ts, std::atomic<bool>& continue_flag)
{
  while (continue_flag.load() &&
         (get_timestamp_estimate() < ts || get_timestamp_estimate() == std::numeric_limits<uint64_t>::max())) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return continue_flag.load() ? TimestampEstimatorBase::kFinished : TimestampEstimatorBase::kInterrupted;
}

} // namespace utilities
} // namespace dunedaq
