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

#include "nodes_registry.h"

#include <spaghetti/logger.h>

#include "nodes/all.h"

namespace spaghetti {

template<>
NodesRegistry &NodesRegistry::get()
{
  static NodesRegistry s_registry{};
  return s_registry;
}

void load_internal_nodes()
{
  auto &registry = NodesRegistry::get();

  registry.registerType<nodes::Package>();
  registry.registerType<nodes::logic::Blinker>();
  registry.registerType<nodes::timers::Clock>();
  registry.registerType<nodes::pneumatic::Tank>();
}

void load_nodes_plugins()
{
  auto &registry = NodesRegistry::get();
  registry.loadPlugins();
}

void load_nodes_meta_data()
{
  auto const &REGISTRY = Registry::get();
  auto &nodesRegistry = NodesRegistry::get();
}

Node *create_node_for(string::hash_t const a_hash)
{
  auto &registry = NodesRegistry::get();

  if (registry.has(a_hash))
    return registry.create(a_hash);

  return new Node;
}

QString node_name_for(string::hash_t const a_hash)
{
  auto const &REGISTRY = NodesRegistry::get();

  assert(REGISTRY.has(a_hash));

  return REGISTRY.data().nodes.at(a_hash).name;
}

QString node_icon_for(string::hash_t const a_hash)
{
  auto const &REGISTRY = NodesRegistry::get();

  assert(REGISTRY.has(a_hash));

  return REGISTRY.data().nodes.at(a_hash).icon;
}

} // namespace spaghetti
