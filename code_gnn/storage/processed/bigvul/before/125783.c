void ParamTraits<string16>::Log(const param_type& p, std::string* l) {
  l->append(UTF16ToUTF8(p));
}
