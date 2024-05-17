bool ParamTraits<double>::Read(const Message* m, PickleIterator* iter,
                               param_type* r) {
  const char *data;
  int data_size;
  if (!m->ReadData(iter, &data, &data_size) ||
      data_size != sizeof(param_type)) {
    NOTREACHED();
    return false;
  }
  memcpy(r, data, sizeof(param_type));
  return true;
}
