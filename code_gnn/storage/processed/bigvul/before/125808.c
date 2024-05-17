bool ParamTraits<NullableString16>::Read(const Message* m, PickleIterator* iter,
                                         param_type* r) {
  string16 string;
  if (!ReadParam(m, iter, &string))
    return false;
  bool is_null;
  if (!ReadParam(m, iter, &is_null))
    return false;
  *r = NullableString16(string, is_null);
  return true;
}
