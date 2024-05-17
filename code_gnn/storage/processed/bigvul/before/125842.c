void ParamTraits<MSG>::Write(Message* m, const param_type& p) {
  m->WriteData(reinterpret_cast<const char*>(&p), sizeof(MSG));
}
