// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <algorithm>

#include "elements/values/scale_int.h"
#include "spaghetti/logger.h"
#include "spaghetti/utils.h"

namespace spaghetti::elements::values {

ScaleInt::ScaleInt()
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eInt, "Value", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  addOutput(ValueType::eInt, "Scaled", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  setIconifyingHidesCentralWidget(true);

  m_series.clear();

  m_series.push_back({ 0, 100 });

  for (size_t i = 1; i < 100; i += 1) {
    float ii = static_cast<float>(i) / 150.f;
    m_series.push_back(vec2{ static_cast<int32_t>(i), static_cast<int32_t>((std::sin(ii * 10.f) * 50.f) + 100.f) });
  }

  m_series.push_back({ 100, m_series[m_series.size() - 1].y });
}

void ScaleInt::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];

  properties["series"] = m_series;
  properties["x_range"] = m_xRange;
  properties["x_ticks"] = m_xTicks;
  properties["y_range"] = m_yRange;
  properties["y_ticks"] = m_yTicks;
}

void ScaleInt::deserialize(const Json &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];

  m_series = PROPERTIES["series"].get<Series>();
  m_xRange = PROPERTIES["x_range"].get<vec2>();
  m_xTicks = PROPERTIES["x_ticks"].get<vec2>();
  m_yRange = PROPERTIES["y_range"].get<vec2>();
  m_yTicks = PROPERTIES["y_ticks"].get<vec2>();
}

void ScaleInt::calculate()
{
  int32_t const INPUT_VALUE{ std::get<int32_t>(m_inputs[0].value) };
  int32_t const VALUE{ std::clamp(INPUT_VALUE, m_xRange.x, m_xRange.y) };

  if (VALUE == m_lastValue) return;

  m_lastValue = VALUE;

  vec2 *previousPtr{ &m_series[0] };
  vec2 *currentPtr{};

  size_t const SIZE = m_series.size();
  for (size_t i = 0; i < SIZE; ++i) {
    currentPtr = &m_series[i];
    if (currentPtr->x < VALUE)
      previousPtr = currentPtr;
    else
      break;
  }

  assert(previousPtr);
  assert(currentPtr);
  vec2 const PREVIOUS{ *previousPtr };
  vec2 const CURRENT{ *currentPtr };

  float value{};

  if (VALUE == PREVIOUS.x)
    value = static_cast<float>(PREVIOUS.y);
  else if (VALUE == CURRENT.x)
    value = static_cast<float>(CURRENT.y);
  else {
    float const PERCENT{ static_cast<float>(VALUE - PREVIOUS.x) / static_cast<float>(CURRENT.x - PREVIOUS.x) };
    value = lerp(static_cast<float>(PREVIOUS.y), static_cast<float>(CURRENT.y), PERCENT);
  }

  m_currentValue.x = VALUE;
  m_currentValue.y = static_cast<int32_t>(value);

  m_outputs[0].value = static_cast<int32_t>(value);
}

void ScaleInt::setSeriesCount(size_t const a_seriesCount)
{
  if (a_seriesCount < 2) return;

  m_series.resize(a_seriesCount);
}

} // namespace spaghetti::elements::values
