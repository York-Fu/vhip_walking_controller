# Changelog

All notable changes to this project will be documented in this file.

## [vhip\_walking\_controller v0.8] - 2019/09/22

### Added

- Desired wrench can be computed with both VHIP or LIPM reduced models
- HRP4ForceCalibrator to decouple normal force and torques while standing
- Stability condition for DCM proportional gain ``k > 1``
- Vertical CoM admittance control to regulate virtual leg stiffness of VHIP

### Changed

- DCM proportional and integral gains have been normalized by ``omega = 3.5455``.

## [lipm\_walking\_controller v1.1] - 2019/04/15

This release corresponds to the stair climbing and walking control that ran in
the final demonstrator of the COMANOID project.

## [lipm\_walking\_controller v1.0] - 2018/10/17

First public release of the controller.
