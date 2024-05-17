void Vec4::SetValues<GLuint>(const GLuint* values) {
  DCHECK(values);
  for (size_t ii = 0; ii < 4; ++ii)
    v_[ii].uint_value = values[ii];
  type_ = SHADER_VARIABLE_UINT;
}
