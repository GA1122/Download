void ParamTraits<content::Referrer>::Log(
    const param_type& p, std::string* l) {
  l->append("(");
  LogParam(p.url, l);
  l->append(",");
  LogParam(p.policy, l);
  l->append(")");
}
