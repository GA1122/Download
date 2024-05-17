int GetMaxLog2TileCols(int sb64_cols) {
  const int kMinTileWidthB64 = 4;
  int max_log2 = 1;
  while ((sb64_cols >> max_log2) >= kMinTileWidthB64)
    max_log2++;
  return max_log2 - 1;
}
