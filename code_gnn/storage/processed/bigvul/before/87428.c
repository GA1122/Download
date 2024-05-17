static unsigned HuffmanTree_makeFromLengths(HuffmanTree* tree, const unsigned* bitlen,
                                            size_t numcodes, unsigned maxbitlen)
{
  unsigned i;
  tree->lengths = (unsigned*)calloc(numcodes, sizeof(unsigned));
  if(!tree->lengths) return 83;  
  for(i = 0; i < numcodes; i++) tree->lengths[i] = bitlen[i];
  tree->numcodes = (unsigned)numcodes;  
  tree->maxbitlen = maxbitlen;
  return HuffmanTree_makeFromLengths2(tree);
}