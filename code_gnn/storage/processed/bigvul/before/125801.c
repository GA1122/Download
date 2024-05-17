bool ParamTraits<unsigned short>::Read(const Message* m, PickleIterator* iter,
                                       param_type* r) {
  const char* data;
  if (!m->ReadBytes(iter, &data, sizeof(param_type)))
    return false;
  memcpy(r, data, sizeof(param_type));
  return true;
}
