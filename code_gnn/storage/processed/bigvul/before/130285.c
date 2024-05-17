static LPITEMIDLIST PIDLNext(LPITEMIDLIST pidl) {
  return reinterpret_cast<LPITEMIDLIST>(
      reinterpret_cast<BYTE*>(pidl) + pidl->mkid.cb);
}
