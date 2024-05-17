static void ConvertBigEndian(HFSPlusForkData* fork) {
  ConvertBigEndian(&fork->logicalSize);
  ConvertBigEndian(&fork->clumpSize);
  ConvertBigEndian(&fork->totalBlocks);
  for (size_t i = 0; i < arraysize(fork->extents); ++i) {
    ConvertBigEndian(&fork->extents[i].startBlock);
    ConvertBigEndian(&fork->extents[i].blockCount);
  }
}
