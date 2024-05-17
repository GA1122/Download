ScopableCPRequest::ScopableCPRequest(const char* u, const char* m,
                                     CPBrowsingContext c) {
  pdata = NULL;
  data = NULL;
#if defined(OS_WIN)
  url = _strdup(u);
  method = _strdup(m);
#else
  url = strdup(u);
  method = strdup(m);
#endif
  context = c;
}
