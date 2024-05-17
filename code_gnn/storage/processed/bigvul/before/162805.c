void Vec4::GetValues<GLuint>(GLuint* values) const {
  DCHECK(values);
  switch (type_) {
    case SHADER_VARIABLE_FLOAT:
      for (size_t ii = 0; ii < 4; ++ii)
        values[ii] = static_cast<GLuint>(v_[ii].float_value);
      break;
    case SHADER_VARIABLE_INT:
      for (size_t ii = 0; ii < 4; ++ii)
        values[ii] = static_cast<GLuint>(v_[ii].int_value);
      break;
    case SHADER_VARIABLE_UINT:
      for (size_t ii = 0; ii < 4; ++ii)
        values[ii] = v_[ii].uint_value;
      break;
    default:
      NOTREACHED();
      break;
  }
}
