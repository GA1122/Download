void ParamTraits<std::wstring>::Log(const param_type& p, std::string* l) {
  l->append(WideToUTF8(p));
}
