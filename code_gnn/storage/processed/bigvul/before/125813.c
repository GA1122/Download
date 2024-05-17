bool ParamTraits<IPC::ChannelHandle>::Read(const Message* m,
                                           PickleIterator* iter,
                                           param_type* r) {
  return ReadParam(m, iter, &r->name)
#if defined(OS_POSIX)
      && ReadParam(m, iter, &r->socket)
#endif
      ;
}
