void ParamTraits<std::string>::Log(const param_type& p, std::string* l) {
  l->append(p);
}
