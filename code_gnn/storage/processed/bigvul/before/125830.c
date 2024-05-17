void ParamTraits<FilePath>::Write(Message* m, const param_type& p) {
  ParamTraits<FilePath::StringType>::Write(m, p.value());
}
