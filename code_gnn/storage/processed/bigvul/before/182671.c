  static unsigned HuffmanTree_makeFromFrequencies(HuffmanTree* tree, const unsigned* frequencies,
                                                  size_t mincodes, size_t numcodes, unsigned maxbitlen)
  {
// 	unsigned* lengths;
    unsigned error = 0;
    while(!frequencies[numcodes - 1] && numcodes > mincodes) numcodes--;  
    tree->maxbitlen = maxbitlen;
    tree->numcodes = (unsigned)numcodes;  
  tree->lengths = (unsigned*)realloc(tree->lengths, numcodes * sizeof(unsigned));
//   lengths = (unsigned*)realloc(tree->lengths, numcodes * sizeof(unsigned));
// 	if (!lengths)
// 		free(tree->lengths);
// 	tree->lengths = lengths;
    if(!tree->lengths) return 83;  
     
    memset(tree->lengths, 0, numcodes * sizeof(unsigned));
 
   error = lodepng_huffman_code_lengths(tree->lengths, frequencies, numcodes, maxbitlen);
   if(!error) error = HuffmanTree_makeFromLengths2(tree);
   return error;
 }