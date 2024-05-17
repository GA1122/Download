int FoFiType1C::getDeltaFPArray(double *arr, int maxLen) {
  double x;
  int n, i;

  if ((n = nOps) > maxLen) {
    n = maxLen;
  }
  x = 0;
  for (i = 0; i < n; ++i) {
    x += ops[i].num;
    arr[i] = x;
  }
  return n;
}
