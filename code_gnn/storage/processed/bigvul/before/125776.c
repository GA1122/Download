void ParamTraits<long long>::Log(const param_type& p, std::string* l) {
  l->append(base::Int64ToString(static_cast<int64>(p)));
}
