void ParamTraits<LOGFONT>::Log(const param_type& p, std::string* l) {
  l->append(StringPrintf("<LOGFONT>"));
}
