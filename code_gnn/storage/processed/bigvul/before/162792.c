bool Vec4::Equal(const Vec4& other) const {
  if (type_ != other.type_)
    return false;
  switch (type_) {
    case SHADER_VARIABLE_FLOAT:
      for (size_t ii = 0; ii < 4; ++ii) {
        if (v_[ii].float_value != other.v_[ii].float_value)
          return false;
      }
      break;
    case SHADER_VARIABLE_INT:
      for (size_t ii = 0; ii < 4; ++ii) {
        if (v_[ii].int_value != other.v_[ii].int_value)
          return false;
      }
      break;
    case SHADER_VARIABLE_UINT:
      for (size_t ii = 0; ii < 4; ++ii) {
        if (v_[ii].uint_value != other.v_[ii].uint_value)
          return false;
      }
      break;
    default:
      NOTREACHED();
      break;
  }
  return true;
}
