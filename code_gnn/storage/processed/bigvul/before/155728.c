 double GamepadBuilder::ApplyAxisDeadzoneToValue(double value) const {
   return std::fabs(value) < axis_deadzone_ ? 0 : value;
 }
