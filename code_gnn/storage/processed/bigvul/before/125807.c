bool ParamTraits<ListValue>::Read(
    const Message* m, PickleIterator* iter, param_type* r) {
  int type;
  if (!ReadParam(m, iter, &type) || type != Value::TYPE_LIST)
    return false;

  return ReadListValue(m, iter, r, 0);
}
