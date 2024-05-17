void ParamTraits<HANDLE>::Log(const param_type& p, std::string* l) {
  l->append(StringPrintf("0x%X", p));
}
