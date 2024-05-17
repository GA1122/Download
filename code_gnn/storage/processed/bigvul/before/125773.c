void ParamTraits<unsigned int>::Log(const param_type& p, std::string* l) {
  l->append(base::UintToString(p));
}
