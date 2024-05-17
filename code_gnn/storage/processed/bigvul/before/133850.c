static void ConvertBigEndian(BTHeaderRec* header) {
  ConvertBigEndian(&header->treeDepth);
  ConvertBigEndian(&header->rootNode);
  ConvertBigEndian(&header->leafRecords);
  ConvertBigEndian(&header->firstLeafNode);
  ConvertBigEndian(&header->lastLeafNode);
  ConvertBigEndian(&header->nodeSize);
  ConvertBigEndian(&header->maxKeyLength);
  ConvertBigEndian(&header->totalNodes);
  ConvertBigEndian(&header->freeNodes);
  ConvertBigEndian(&header->reserved1);
  ConvertBigEndian(&header->clumpSize);
  ConvertBigEndian(&header->attributes);
}
