#ifndef PIPE_NETWORK_VALVE_GRAPH_H
#define PIPE_NETWORK_VALVE_GRAPH_H

#include "mesh_components.h"
namespace pipenetwork {

//! Isolate Valve Property
//! name Valve name
//! on_node node name that the valve is closed to
//! on_pipe pipe name that the valve is on
struct ISOVProp {
  std::string name{"junction"};
  std::string on_node{"Null"};
  std::string on_pipe{"Null"};
};

struct IsoSeg {
  std::set<Index> pids;
  std::set<Index> nids;
  std::set<std::string> vnames;
};

class ValveGraph : MeshGraph {
 public:
  ValveGraph(const std::shared_ptr<MeshNodes>& mesh_nodes,
             const std::shared_ptr<MeshLinks>& mesh_links,
             const std::vector<ISOVProp>& iso_valve_props);

  Eigen::SparseMatrix<int>& node_pipe_mtx() { return node_pipe_mtx_; }

  Eigen::SparseMatrix<int>& valve_loc_mtx() { return valve_loc_mtx_; }

  Eigen::SparseMatrix<int>& valve_def_mtx() { return valve_def_mtx_; }

  //! get the corresponding isolation segment from a pipe
  IsoSeg get_iso_seg(Index pid);

 private:
  //! node pipe matrix
  Eigen::SparseMatrix<int> node_pipe_mtx_;
  //! valve location matrix
  Eigen::SparseMatrix<int> valve_loc_mtx_;
  //! valve deficiency matrix
  Eigen::SparseMatrix<int> valve_def_mtx_;
  //! valve name map with the position in the valve location matrix as the key
  std::map<std::pair<Index, Index>, std::string> id2vname_;

  //! construct the node_pipe_mtx_
  void construct_node_pipe_mtx();

  //! construct the valve location matrix
  void construct_valve_loc_mtx(const std::vector<ISOVProp>& iso_valve_props);

  //! construct the valve defeciency matrix
  void construct_valve_def_mtx();

  //! construct the col2row, row2col look up tables
  void construct_idx_table();

  std::map<Index, std::vector<Index>> col2row_;
  std::map<Index, std::vector<Index>> row2col_;
};

}  // namespace pipenetwork

#endif  // PIPE_NETWORK_VALVE_GRAPH_H