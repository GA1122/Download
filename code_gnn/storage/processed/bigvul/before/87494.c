static unsigned huffmanDecodeSymbol(const unsigned char* in, size_t* bp,
                                    const HuffmanTree* codetree, size_t inbitlength)
{
  unsigned treepos = 0, ct;
  if (!codetree || !codetree->tree2d)
    return 0;

  for(;;)
  {
    if(*bp >= inbitlength) return (unsigned)(-1);  
     
    ct = codetree->tree2d[(treepos << 1) + READBIT(*bp, in)];
    (*bp)++;
    if(ct < codetree->numcodes) return ct;  
    else treepos = ct - codetree->numcodes;  

    if(treepos >= codetree->numcodes) return (unsigned)(-1);  
  }
}
