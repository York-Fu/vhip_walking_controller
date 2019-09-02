/* Copyright 2018-2019 CNRS-UM LIRMM
 *
 * \author Stéphane Caron
 *
 * This file is part of vhip_walking_controller.
 *
 * vhip_walking_controller is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * vhip_walking_controller is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with vhip_walking_controller. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <vhip_walking/Pendulum.h>

namespace vhip_walking
{
  Pendulum::Pendulum(const Eigen::Vector3d & com, const Eigen::Vector3d & comd, const Eigen::Vector3d & comdd)
  {
    reset(com, comd, comdd);
  }

  void Pendulum::reset(const Eigen::Vector3d & com, const Eigen::Vector3d & comd, const Eigen::Vector3d & comdd)
  {
    constexpr double DEFAULT_LAMBDA = world::GRAVITY / 0.8;
    com_ = com;
    comd_ = comd;
    comdd_ = comdd;
    comddd_ = Eigen::Vector3d::Zero();
    omega_ = std::sqrt(DEFAULT_LAMBDA);
    zmp_ = com_ - comdd_ / DEFAULT_LAMBDA;
    zmpd_ = comd_ - comddd_ / DEFAULT_LAMBDA;
  }

  void Pendulum::integrateIPM(Eigen::Vector3d zmp, double lambda, double dt)
  {
    Eigen::Vector3d com_prev = com_;
    Eigen::Vector3d comd_prev = comd_;
    omega_ = std::sqrt(lambda);
    zmp_ = zmp;

    Eigen::Vector3d vrp = zmp_ - world::gravity / lambda;
    double ch = std::cosh(omega_ * dt);
    double sh = std::sinh(omega_ * dt);
    comdd_ = lambda * (com_prev - zmp_) + world::gravity;
    comd_ = comd_prev * ch + omega_ * (com_prev - vrp) * sh;
    com_ = com_prev * ch + comd_prev * sh / omega_ - vrp * (ch - 1.0);

    // default values for third-order terms
    comddd_ = Eigen::Vector3d::Zero();
    zmpd_ = comd_ - comddd_ / lambda;
  }

  void Pendulum::integrateCoMJerk(const Eigen::Vector3d & comddd, double dt)
  {
    com_ += dt * (comd_ + dt * (comdd_ / 2 + dt * (comddd / 6)));
    comd_ += dt * (comdd_ + dt * (comddd / 2));
    comdd_ += dt * comddd;
    comddd_ = comddd;
  }

  void Pendulum::resetCoMHeight(double height, const Contact & plane)
  {
    auto n = plane.normal();
    com_ += (height + n.dot(plane.p() - com_)) * n;
    comd_ -= n.dot(comd_) * n;
    comdd_ -= n.dot(comdd_) * n;
    comddd_ -= n.dot(comddd_) * n;
  }

  void Pendulum::completeIPM(const Contact & plane)
  {
    auto n = plane.normal();
    auto gravitoInertial = world::gravity - comdd_;
    double lambda = n.dot(gravitoInertial) / n.dot(plane.p() - com_);
    zmp_ = com_ + gravitoInertial / lambda;
    zmpd_ = comd_ - comddd_ / lambda;
    omega_ = std::sqrt(lambda);
  }
}
