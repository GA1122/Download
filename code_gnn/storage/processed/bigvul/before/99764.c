 int GetMinLog2TileCols(int sb64_cols) {
  const int kMaxTileWidthB64 = 64;
  int min_log2 = 0;
  while ((kMaxTileWidthB64 << min_log2) < sb64_cols)
    min_log2++;
  return min_log2;
}
