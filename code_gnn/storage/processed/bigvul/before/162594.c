 void ParamTraits<gfx::Transform>::Write(base::Pickle* m, const param_type& p) {
#ifdef SK_MSCALAR_IS_FLOAT
  float column_major_data[16];
  p.matrix().asColMajorf(column_major_data);
#else
  double column_major_data[16];
  p.matrix().asColMajord(column_major_data);
#endif
  m->WriteBytes(&column_major_data, sizeof(SkMScalar) * 16);
}
