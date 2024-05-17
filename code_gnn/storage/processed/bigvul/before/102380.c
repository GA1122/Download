struct addrinfo* CreateCopyOfAddrinfo(const struct addrinfo* info,
                                      bool recursive) {
  DCHECK(info);
  struct addrinfo* copy = new addrinfo;

  memcpy(copy, info, sizeof(addrinfo));

  if (info->ai_canonname) {
    copy->ai_canonname = do_strdup(info->ai_canonname);
  }

  if (info->ai_addr) {
    copy->ai_addr = reinterpret_cast<sockaddr *>(new char[info->ai_addrlen]);
    memcpy(copy->ai_addr, info->ai_addr, info->ai_addrlen);
  }

  if (recursive && info->ai_next)
    copy->ai_next = CreateCopyOfAddrinfo(info->ai_next, recursive);
  else
    copy->ai_next = NULL;

  return copy;
}
