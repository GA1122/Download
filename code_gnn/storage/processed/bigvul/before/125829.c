void ParamTraits<base::FileDescriptor>::Write(Message* m, const param_type& p) {
  const bool valid = p.fd >= 0;
  WriteParam(m, valid);

  if (valid) {
    if (!m->WriteFileDescriptor(p))
      NOTREACHED();
  }
}
