static LPITEMIDLIST GetNthPIDL(CIDA* cida, int n) {
  return reinterpret_cast<LPITEMIDLIST>(
      reinterpret_cast<LPBYTE>(cida) + cida->aoffset[n]);
}
