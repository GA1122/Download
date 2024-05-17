void ParamTraits<std::vector<char> >::Write(Message* m, const param_type& p) {
  if (p.empty()) {
    m->WriteData(NULL, 0);
  } else {
    m->WriteData(&p.front(), static_cast<int>(p.size()));
  }
}
