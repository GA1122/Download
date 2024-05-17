void FreeCopyOfAddrinfo(struct addrinfo* info) {
  DCHECK(info);
  if (info->ai_canonname)
    free(info->ai_canonname);   

  if (info->ai_addr)
    delete [] reinterpret_cast<char*>(info->ai_addr);

  struct addrinfo* next = info->ai_next;

  delete info;

  if (next)
    FreeCopyOfAddrinfo(next);
}
