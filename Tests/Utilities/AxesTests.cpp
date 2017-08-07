// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#define BOOST_TEST_MODULE grid axis tests
#include <boost/test/included/unit_test.hpp>

#include "ACTS/Utilities/detail/Axis.hpp"

namespace Acts {

using namespace detail;

namespace Test {

  BOOST_AUTO_TEST_CASE(equidistant_axis)
  {
    EquidistantAxis a(0.0, 10.0, 10u);

    // general binning properties
    BOOST_TEST(a.getNBins() == 10u);
    BOOST_TEST(a.getMax() == 10.);
    BOOST_TEST(a.getMin() == 0.);
    BOOST_TEST(a.getBinWidth() == 1.);

    // bin index calculation
    BOOST_TEST(a.getBin(-0.3) == 0u);
    BOOST_TEST(a.getBin(-0.) == 1u);
    BOOST_TEST(a.getBin(0.) == 1u);
    BOOST_TEST(a.getBin(0.7) == 1u);
    BOOST_TEST(a.getBin(1) == 2u);
    BOOST_TEST(a.getBin(1.2) == 2u);
    BOOST_TEST(a.getBin(2.) == 3u);
    BOOST_TEST(a.getBin(2.7) == 3u);
    BOOST_TEST(a.getBin(3.) == 4u);
    BOOST_TEST(a.getBin(3.6) == 4u);
    BOOST_TEST(a.getBin(4.) == 5u);
    BOOST_TEST(a.getBin(4.98) == 5u);
    BOOST_TEST(a.getBin(5.) == 6u);
    BOOST_TEST(a.getBin(5.12) == 6u);
    BOOST_TEST(a.getBin(6.) == 7u);
    BOOST_TEST(a.getBin(6.00001) == 7u);
    BOOST_TEST(a.getBin(7.) == 8u);
    BOOST_TEST(a.getBin(7.5) == 8u);
    BOOST_TEST(a.getBin(8.) == 9u);
    BOOST_TEST(a.getBin(8.1) == 9u);
    BOOST_TEST(a.getBin(9.) == 10u);
    BOOST_TEST(a.getBin(9.999) == 10u);
    BOOST_TEST(a.getBin(10.) == 11u);
    BOOST_TEST(a.getBin(100.3) == 11u);

    // lower bin boundaries
    BOOST_TEST(a.getBinLowerBound(1) == 0.);
    BOOST_TEST(a.getBinLowerBound(2) == 1.);
    BOOST_TEST(a.getBinLowerBound(3) == 2.);
    BOOST_TEST(a.getBinLowerBound(4) == 3.);
    BOOST_TEST(a.getBinLowerBound(5) == 4.);
    BOOST_TEST(a.getBinLowerBound(6) == 5.);
    BOOST_TEST(a.getBinLowerBound(7) == 6.);
    BOOST_TEST(a.getBinLowerBound(8) == 7.);
    BOOST_TEST(a.getBinLowerBound(9) == 8.);
    BOOST_TEST(a.getBinLowerBound(10) == 9.);

    // upper bin boundaries
    BOOST_TEST(a.getBinUpperBound(1) == 1.);
    BOOST_TEST(a.getBinUpperBound(2) == 2.);
    BOOST_TEST(a.getBinUpperBound(3) == 3.);
    BOOST_TEST(a.getBinUpperBound(4) == 4.);
    BOOST_TEST(a.getBinUpperBound(5) == 5.);
    BOOST_TEST(a.getBinUpperBound(6) == 6.);
    BOOST_TEST(a.getBinUpperBound(7) == 7.);
    BOOST_TEST(a.getBinUpperBound(8) == 8.);
    BOOST_TEST(a.getBinUpperBound(9) == 9.);
    BOOST_TEST(a.getBinUpperBound(10) == 10.);

    // bin centers
    BOOST_TEST(a.getBinCenter(1) == 0.5);
    BOOST_TEST(a.getBinCenter(2) == 1.5);
    BOOST_TEST(a.getBinCenter(3) == 2.5);
    BOOST_TEST(a.getBinCenter(4) == 3.5);
    BOOST_TEST(a.getBinCenter(5) == 4.5);
    BOOST_TEST(a.getBinCenter(6) == 5.5);
    BOOST_TEST(a.getBinCenter(7) == 6.5);
    BOOST_TEST(a.getBinCenter(8) == 7.5);
    BOOST_TEST(a.getBinCenter(9) == 8.5);
    BOOST_TEST(a.getBinCenter(10) == 9.5);

    // inside check
    BOOST_TEST(not a.isInside(-0.2));
    BOOST_TEST(a.isInside(0.));
    BOOST_TEST(a.isInside(3.));
    BOOST_TEST(not a.isInside(10.));
    BOOST_TEST(not a.isInside(12.));
  }

  BOOST_AUTO_TEST_CASE(variable_axis)
  {
    VariableAxis a({0, 0.5, 3, 4.5, 6});

    // general binning properties
    BOOST_TEST(a.getNBins() == 4u);
    BOOST_TEST(a.getMax() == 6.);
    BOOST_TEST(a.getMin() == 0.);

    // bin index calculation
    BOOST_TEST(a.getBin(-0.3) == 0u);
    BOOST_TEST(a.getBin(-0.) == 1u);
    BOOST_TEST(a.getBin(0.) == 1u);
    BOOST_TEST(a.getBin(0.3) == 1u);
    BOOST_TEST(a.getBin(0.5) == 2u);
    BOOST_TEST(a.getBin(1.2) == 2u);
    BOOST_TEST(a.getBin(2.7) == 2u);
    BOOST_TEST(a.getBin(3.) == 3u);
    BOOST_TEST(a.getBin(4.49999) == 3u);
    BOOST_TEST(a.getBin(4.5) == 4u);
    BOOST_TEST(a.getBin(5.12) == 4u);
    BOOST_TEST(a.getBin(6.) == 5u);
    BOOST_TEST(a.getBin(6.00001) == 5u);
    BOOST_TEST(a.getBin(7.5) == 5u);

    // lower bin boundaries
    BOOST_TEST(a.getBinLowerBound(1) == 0.);
    BOOST_TEST(a.getBinLowerBound(2) == 0.5);
    BOOST_TEST(a.getBinLowerBound(3) == 3.);
    BOOST_TEST(a.getBinLowerBound(4) == 4.5);

    // upper bin boundaries
    BOOST_TEST(a.getBinUpperBound(1) == 0.5);
    BOOST_TEST(a.getBinUpperBound(2) == 3.);
    BOOST_TEST(a.getBinUpperBound(3) == 4.5);
    BOOST_TEST(a.getBinUpperBound(4) == 6.);

    // bin centers
    BOOST_TEST(a.getBinCenter(1) == 0.25);
    BOOST_TEST(a.getBinCenter(2) == 1.75);
    BOOST_TEST(a.getBinCenter(3) == 3.75);
    BOOST_TEST(a.getBinCenter(4) == 5.25);

    // inside check
    BOOST_TEST(not a.isInside(-0.2));
    BOOST_TEST(a.isInside(0.));
    BOOST_TEST(a.isInside(3.));
    BOOST_TEST(not a.isInside(6.));
    BOOST_TEST(not a.isInside(12.));
  }
}  // namespace Test

}  // namespace Acts