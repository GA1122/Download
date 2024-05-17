void ParamTraits<IPC::ChannelHandle>::Log(const param_type& p,
                                          std::string* l) {
  l->append(StringPrintf("ChannelHandle(%s", p.name.c_str()));
#if defined(OS_POSIX)
  l->append(", ");
  ParamTraits<base::FileDescriptor>::Log(p.socket, l);
#endif
  l->append(")");
}
