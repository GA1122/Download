static size_t PIDLSize(LPITEMIDLIST pidl) {
  size_t s = 0;
  while (pidl->mkid.cb > 0) {
    s += pidl->mkid.cb;
    pidl = PIDLNext(pidl);
  }
  return 2 + s;
}
