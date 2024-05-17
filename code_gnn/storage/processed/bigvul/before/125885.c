void ParamTraits<content::Referrer>::Write(
    Message* m, const param_type& p) {
  WriteParam(m, p.url);
  WriteParam(m, p.policy);
}
