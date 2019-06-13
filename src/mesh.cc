#include "mesh.h"

void pipenetwork::Mesh::create_junctions(
    const std::vector<Index>& ids, const std::vector<double>& elevations,
    const std::vector<double>& demands,
    const std::vector<double>& leak_diameters) {

  Index i = 0;
  for (const auto& id : ids) {
    nodes_.emplace(id, std::make_shared<pipenetwork::Junction>(
                           id, elevations[i], demands[i], leak_diameters[i]));
    ++i;
  }
}

void pipenetwork::Mesh::create_reservoirs(const std::vector<Index>& ids,
                                          const std::vector<double>& heads) {
  Index i = 0;
  for (const auto& id : ids) {
    nodes_.emplace(id, std::make_shared<pipenetwork::Reservoir>(id, heads[i]));
    ++i;
  }
}

bool pipenetwork::Mesh::create_pipes(
    const std::vector<Index>& ids,
    const std::vector<std::pair<Index, Index>>& nodeids,
    const std::vector<double>& length, const std::vector<double>& diameter,
    const std::vector<double>& roughness,
    const std::vector<Pipe_status>& status) {

  Index i = 0;
  bool stat = true;
  try {
    for (const auto& id : ids) {
      auto node1id = nodeids[i].first;
      auto node2id = nodeids[i].second;
      connected_nodes_.emplace(node1id, nodes_.at(node1id));
      connected_nodes_.emplace(node2id, nodes_.at(node2id));

      links_.emplace(id, std::make_shared<pipenetwork::Pipe>(
                             id, nodes_.at(node1id), nodes_.at(node2id),
                             length[i], diameter[i], roughness[i], status[i]));
      ++i;
    }

  } catch (std::out_of_range& range_error) {
    std::cout << "Input node does not exist, nodal id: " << nodeids.at(i).first
              << " or " << nodeids.at(i).second << '\n'
              << "Pipe creation is unfinished" << '\n';
    stat = false;
  }
  return stat;
}

void pipenetwork::Mesh::print_summary() {
  auto nnode = nodes_.size();

  std::cout << "number of nodes: " << nnode << " ;number of pipes: " << nlinks()
            << " ;number of connected nodes: " << nnodes() << std::endl;

  std::cout
      << "======================= CONNECTED NODE INFO ======================="
      << std::endl;

  for (auto const& x : connected_nodes_) {
    std::cout << x.first << ':' << "; node id: " << x.second->id() << std::endl;
  }

  std::cout << "======================= PIPE INFO ======================="
            << std::endl;
  for (auto const& x : links_) {
    std::cout << x.first << ':' << "; pipe id: " << x.second->id()
              << "; node1 id: " << x.second->nodes().first->id()
              << "; node2 id: " << x.second->nodes().second->id() << std::endl;
  }
}
