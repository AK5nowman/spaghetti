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

#ifndef CORE_REGISTRY_H
#define CORE_REGISTRY_H

#include <sparsepp/spp.h>
#include <map>
#include "core/metadata.h"
#include "core/strings.h"
#include "elements/element.h"

#define REGISTRY_SPP_MAP 1
#define REGISTRY_STD_MAP 2
#define REGISTRY_MAP REGISTRY_SPP_MAP

namespace core {

class Registry final {
  struct Info {
    MetaData *data;
    using CloneFunc = elements::Element *(*)();
    CloneFunc clone{};
  };

 public:
#if REGISTRY_MAP == REGISTRY_SPP_MAP
  using Elements = spp::sparse_hash_map<string::hash_t, Info>;
#elif REGISTRY_MAP == REGISTRY_STD_MAP
  using Elements = std::map<string::hash_t, Info>;
#endif

  static Registry &get();

  template<typename T>
  void registerElement()
  {
    MetaData &metaData{ T::metaData() };
    string::hash_t const hash{ string::hash(metaData.path) };

    if (m_elements.find(hash) != std::end(m_elements)) return;

    Info info{ &metaData, &clone<T> };
    m_elements[hash] = info;
  }

  elements::Element *create(char const *const a_name) { return create(string::hash(a_name)); }

  elements::Element *create(string::hash_t const a_hash)
  {
    if (m_elements.find(a_hash) == std::end(m_elements)) return nullptr;
    assert(m_elements[a_hash].clone);
    return m_elements[a_hash].clone();
  }

  Elements const &elements() const { return m_elements; }

 private:
  Registry() = default;

  template<typename T>
  static elements::Element *clone()
  {
    return new T;
  }

 private:
  Elements m_elements{};
};

void register_internal_elements();

} // namespace core

#endif // CORE_REGISTRY_H
