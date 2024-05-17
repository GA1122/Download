void Vec4::SetValues<GLint>(const GLint* values) {
  DCHECK(values);
  for (size_t ii = 0; ii < 4; ++ii)
    v_[ii].int_value = values[ii];
  type_ = SHADER_VARIABLE_INT;
}
