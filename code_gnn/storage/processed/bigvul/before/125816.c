bool ParamTraits<HANDLE>::Read(const Message* m, PickleIterator* iter,
                               param_type* r) {
  uint32 temp;
  if (!m->ReadUInt32(iter, &temp))
    return false;
  *r = reinterpret_cast<HANDLE>(temp);
  return true;
}
