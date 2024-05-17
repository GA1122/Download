bool ParamTraits<MSG>::Read(const Message* m, PickleIterator* iter,
                            param_type* r) {
  const char *data;
  int data_size = 0;
  bool result = m->ReadData(iter, &data, &data_size);
  if (result && data_size == sizeof(MSG)) {
    memcpy(r, data, sizeof(MSG));
  } else {
    result = false;
    NOTREACHED();
  }

  return result;
}
