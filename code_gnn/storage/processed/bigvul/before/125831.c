void ParamTraits<ListValue>::Write(Message* m, const param_type& p) {
  WriteValue(m, &p, 0);
}
