void ParamTraits<base::TimeDelta>::Log(const param_type& p, std::string* l) {
  ParamTraits<int64>::Log(p.ToInternalValue(), l);
}
