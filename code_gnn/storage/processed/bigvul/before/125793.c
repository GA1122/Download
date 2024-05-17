void ParamTraits<base::TimeTicks>::Log(const param_type& p, std::string* l) {
  ParamTraits<int64>::Log(p.ToInternalValue(), l);
}
