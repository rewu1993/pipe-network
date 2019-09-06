#ifndef PIPE_NETWORK_HYDRALIC_SIM_H
#define PIPE_NETWORK_HYDRALIC_SIM_H
#include <fstream>
#include <string>
#include <vector>

//#include "eigen_cg.h"
//#include "eigen_cg_lem.h"
//#include "eigen_gmres.h"
#include "input.h"
#include "matrix_assembler.h"
#include "pardiso_unsym.h"
#include "mkl_unsym.h"
#include "settings.h"
#include "output.h"

namespace pipenetwork {
//! Hydraulic Simulation class
//! \brief Class for pipenetwork hydraulic simulation
class Hydralic_sim {
 public:
  //! Constructor with mesh input
  Hydralic_sim(const std::shared_ptr<Mesh>& mesh,
               std::shared_ptr<Curves>& curves_info, bool pdd_mode = false,
               bool debug = false) {
    mesh_ = mesh;
    assembler_ = std::make_shared<MatrixAssembler>(mesh, curves_info, pdd_mode);
    solver_ = std::make_shared<Mkl_unsym>();
    debug_ = debug;
  };
  //! Constructor with .inp file path
  explicit Hydralic_sim(const std::string& filepath, const std::string& mesh_name, bool pdd_mode = false,
                        bool debug = false);
  //! Constructor for synthetic net as input
  Hydralic_sim(int syn_size, bool pdd_mode, bool debug = false);

  //! run simulation
  bool run_simulation(double NR_tolerance = 1.e-8, int max_nr_steps = 1000, bool line_search = true,
                      std::string output_path = "../benchmarks/res_");
  //! get the norm of simulation residual
  double sim_residual_norm() const { return residual_norm_; }

  //! line search
  void line_search_func(const Eigen::VectorXd & x_diff);

 private:
    //! original variable vector (for line search
    Eigen::VectorXd original_variable_;
  //! the mesh ptr
  std::shared_ptr<Mesh> mesh_;
  //! the assember ptr
  std::shared_ptr<MatrixAssembler> assembler_;
  //! the solver ptr
  std::shared_ptr<Mkl_unsym> solver_;
  //! variable vector
  std::shared_ptr<Eigen::VectorXd> variables_;
  //! residual vector
  std::shared_ptr<Eigen::VectorXd> residuals_;
  //! solver tolerance
  double inner_solver_tolerance_{1e-8};
  //! iteration steps
  int max_solver_steps_{10000};
  //! residual norm
  double residual_norm_{std::numeric_limits<float>::min()};
  //! initial discharge
  double init_discharge_{1e-3};
  //! debug flag
  bool debug_{false};
  //! alpha for backtracking
  int bt_max_iter_{20};
  double bt_roh_{0.5};

  void write_final_result(const std::string& output_path,
                          const Eigen::VectorXd& var);
};

}  // namespace pipenetwork

#endif  // PIPE_NETWORK_HYDRALIC_SIM_H
