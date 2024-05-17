void ParamTraits<unsigned long>::Log(const param_type& p, std::string* l) {
  l->append(base::Uint64ToString(static_cast<uint64>(p)));
}
