bool ParamTraits<base::Time>::Read(const Message* m, PickleIterator* iter,
                                   param_type* r) {
  int64 value;
  if (!ParamTraits<int64>::Read(m, iter, &value))
    return false;
  *r = base::Time::FromInternalValue(value);
  return true;
}
