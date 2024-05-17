void Vec4::SetValues<GLfloat>(const GLfloat* values) {
  DCHECK(values);
  for (size_t ii = 0; ii < 4; ++ii)
    v_[ii].float_value = values[ii];
  type_ = SHADER_VARIABLE_FLOAT;
}
