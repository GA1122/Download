void ParamTraits<NullableString16>::Write(Message* m, const param_type& p) {
  WriteParam(m, p.string());
  WriteParam(m, p.is_null());
}
