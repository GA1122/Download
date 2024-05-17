void ParamTraits<unsigned long long>::Log(const param_type& p, std::string* l) {
  l->append(base::Uint64ToString(p));
}
