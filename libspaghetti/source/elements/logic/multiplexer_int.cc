// MIT License
//
// Copyright (c) 2017 Artur Wyszyński, aljen at hitomi dot pl
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

#include "elements/logic/multiplexer_int.h"
#include "spaghetti/utils.h"

namespace spaghetti::elements::logic {

MultiplexerInt::MultiplexerInt()
  : Element{}
{
  setMinInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eInt, "Select");
  addInput(ValueType::eInt, "#1");
  addInput(ValueType::eInt, "#2");

  addOutput(ValueType::eInt, "Value");
}

void MultiplexerInt::calculate()
{
  int32_t const SELECT{ std::get<int32_t>(m_inputs[0].value) };
  int32_t const SIZE{ static_cast<int32_t>(m_inputs.size()) - 2 };
  int32_t const INDEX{ std::clamp<int32_t>(SELECT, 0, SIZE) };
  int32_t const VALUE{ std::get<int32_t>(m_inputs[static_cast<size_t>(INDEX) + 1].value) };

  m_outputs[0].value = VALUE;
}

} // namespace spaghetti::elements::logic
