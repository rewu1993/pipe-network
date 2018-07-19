// Constructor with id and node pointers
Pipe::Pipe(unsigned id, const std::array<std::shared_ptr<Node>, 2>& nodes)
    : id_{id}, nodes_{nodes} {
  length_ = (nodes_.at(0)->coordinates() - nodes_.at(1)->coordinates()).norm();
}

//! Calculate and return discharge using Darcy-Weisbach equation
double Pipe::discharge() {
  if (nodes_[0]->ishead() && nodes_[1]->ishead()) {
    const double dhead = nodes_[0]->head() - nodes_[1]->head();
    discharge_ = sqrt(abs(dhead) * pow(M_PI, 2) * g_ * pow(2 * radius_, 5) /
                      (8 * darcy_friction_));
    if (dhead < 0) {
      discharge_ = -1 * discharge_;
    }
    return discharge_;
  } else {
    throw std::runtime_error(
        "Unknown head exists, cannot calculate discharge using Darcy Weisbach "
        "equation");
  }
}
