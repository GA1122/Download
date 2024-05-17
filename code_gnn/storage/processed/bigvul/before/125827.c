void ParamTraits<std::vector<bool> >::Write(Message* m, const param_type& p) {
  WriteParam(m, static_cast<int>(p.size()));
  for (size_t i = 0; i < p.size(); i++)
    WriteParam(m, p[i]);
}
