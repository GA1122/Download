void ParamTraits<gfx::Transform>::Log(
    const param_type& p, std::string* l) {
#ifdef SK_MSCALAR_IS_FLOAT
  float row_major_data[16];
  p.matrix().asRowMajorf(row_major_data);
#else
  double row_major_data[16];
  p.matrix().asRowMajord(row_major_data);
#endif
  l->append("(");
  for (int i = 0; i < 16; ++i) {
    if (i > 0)
      l->append(", ");
    LogParam(row_major_data[i], l);
  }
  l->append(") ");
}
