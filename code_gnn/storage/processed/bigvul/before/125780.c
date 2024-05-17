void ParamTraits<double>::Log(const param_type& p, std::string* l) {
  l->append(StringPrintf("%e", p));
}
