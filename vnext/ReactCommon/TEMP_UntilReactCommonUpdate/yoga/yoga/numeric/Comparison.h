/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cmath>

#include <yoga/Yoga.h>

namespace facebook::yoga {

template <typename T>
constexpr bool isUndefined(T value) {
  return value != value;
}

// [Windows c++20 fix #12195]
template <typename T>
constexpr bool isDefined(T value) {
  return !isUndefined(value);
}

template <typename T>
constexpr auto maxOrDefined(T a, T b) {
  if (yoga::isDefined(a) && yoga::isDefined(b)) {
    return std::max(a, b);
  }
  return yoga::isUndefined(a) ? b : a;
}

template <typename T>
constexpr auto minOrDefined(T a, T b) {
  if (yoga::isDefined(a) && yoga::isDefined(b)) {
    return std::min(a, b);
  }

  return yoga::isUndefined(a) ? b : a;
}

// Custom equality functions using a hardcoded epsilon of 0.0001f, or returning
// true if both floats are NaN.
inline bool inexactEquals(float a, float b) {
  if (yoga::isDefined(a) && yoga::isDefined(b)) {
    return std::abs(a - b) < 0.0001f;
  }
  return yoga::isUndefined(a) && yoga::isUndefined(b);
}

inline bool inexactEquals(double a, double b) {
 if (yoga::isDefined(a) && yoga::isDefined(b)) {
    return std::abs(a - b) < 0.0001;
  }
  return yoga::isUndefined(a) && yoga::isUndefined(b);
}

inline bool inexactEquals(const YGValue& a, const YGValue& b) {
  if (a.unit != b.unit) {
    return false;
  }

  if (a.unit == YGUnitUndefined ||
      (yoga::isUndefined(a.value) && yoga::isUndefined(b.value))) {
    return true;
  }

  return fabs(a.value - b.value) < 0.0001f;
}

template <std::size_t Size, typename ElementT>
bool inexactEquals(
    const std::array<ElementT, Size>& val1,
    const std::array<ElementT, Size>& val2) {
  bool areEqual = true;
  for (std::size_t i = 0; i < Size && areEqual; ++i) {
    areEqual = inexactEquals(val1[i], val2[i]);
  }
  return areEqual;
}

} // namespace facebook::yoga
