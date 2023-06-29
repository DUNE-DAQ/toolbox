/**
 * @file TimestampEstimatorSystem_test.cxx  TimestampEstimatorSystem class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "utilities/TimestampEstimatorSystem.hpp"

/**
 * @brief Name of this test module
 */
#define BOOST_TEST_MODULE TimestampEstimatorSystem_test // NOLINT

#include "boost/test/unit_test.hpp"
#include <boost/test/tools/old/interface.hpp>
#include <chrono>

BOOST_AUTO_TEST_SUITE(BOOST_TEST_MODULE)

BOOST_AUTO_TEST_CASE(Basics)
{
  const uint64_t clock_frequency_hz = 62'500'000; // NOLINT(build/unsigned)
  std::atomic<bool> continue_flag{ true };
  dunedaq::utilities::TimestampEstimatorSystem tes(clock_frequency_hz);

  BOOST_CHECK_EQUAL(tes.wait_for_valid_timestamp(continue_flag),
                    dunedaq::utilities::TimestampEstimatorBase::kFinished);

  std::atomic<bool> do_not_continue_flag{ false };
  BOOST_CHECK_EQUAL(tes.wait_for_valid_timestamp(do_not_continue_flag),
                    dunedaq::utilities::TimestampEstimatorBase::kInterrupted);

  dunedaq::daqdataformats::timestamp_t ts_now = tes.get_timestamp_estimate();
  BOOST_CHECK_EQUAL(tes.wait_for_timestamp(ts_now + clock_frequency_hz, continue_flag),
                    dunedaq::utilities::TimestampEstimatorBase::kFinished);

  ts_now = tes.get_timestamp_estimate();
  BOOST_CHECK_EQUAL(tes.wait_for_timestamp(ts_now + clock_frequency_hz, do_not_continue_flag),
                    dunedaq::utilities::TimestampEstimatorBase::kInterrupted);

  // Check that the timestamp doesn't go backwards
  dunedaq::daqdataformats::timestamp_t ts1 = tes.get_timestamp_estimate();
  dunedaq::daqdataformats::timestamp_t ts2 = tes.get_timestamp_estimate();
  BOOST_CHECK_GE(ts2, ts1);
}

BOOST_AUTO_TEST_SUITE_END()
