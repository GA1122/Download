void ParamTraits<HANDLE>::Write(Message* m, const param_type& p) {
  m->WriteUInt32(reinterpret_cast<uint32>(p));
}
