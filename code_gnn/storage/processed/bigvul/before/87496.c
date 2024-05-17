static unsigned inflateHuffmanBlock(ucvector* out, const unsigned char* in, size_t* bp,
                                    size_t* pos, size_t inlength, unsigned btype)
{
  unsigned error = 0;
  HuffmanTree tree_ll;  
  HuffmanTree tree_d;  
  size_t inbitlength = inlength * 8;

  HuffmanTree_init(&tree_ll);
  HuffmanTree_init(&tree_d);

  if(btype == 1)
  {
     error = getTreeInflateFixed(&tree_ll, &tree_d);
     if (error)
     {
       HuffmanTree_cleanup(&tree_ll);
       HuffmanTree_cleanup(&tree_d);
       return error;
     }
  }
  else if(btype == 2) error = getTreeInflateDynamic(&tree_ll, &tree_d, in, bp, inlength);

  while(!error)  
  {
     
    unsigned code_ll = huffmanDecodeSymbol(in, bp, &tree_ll, inbitlength);
    if(code_ll <= 255)  
    {
       
      if(!ucvector_resize(out, (*pos) + 1)) ERROR_BREAK(83  );
      out->data[*pos] = (unsigned char)code_ll;
      (*pos)++;
    }
    else if(code_ll >= FIRST_LENGTH_CODE_INDEX && code_ll <= LAST_LENGTH_CODE_INDEX)  
    {
      unsigned code_d, distance;
      unsigned numextrabits_l, numextrabits_d;  
      size_t start, forward, backward, length;

       
      length = LENGTHBASE[code_ll - FIRST_LENGTH_CODE_INDEX];

       
      numextrabits_l = LENGTHEXTRA[code_ll - FIRST_LENGTH_CODE_INDEX];
      if(*bp >= inbitlength) ERROR_BREAK(51);  
      length += readBitsFromStream(bp, in, numextrabits_l);

       
      code_d = huffmanDecodeSymbol(in, bp, &tree_d, inbitlength);
      if(code_d > 29)
      {
        if(code_ll == (unsigned)(-1))  
        {
           
          error = (*bp) > inlength * 8 ? 10 : 11;
        }
        else error = 18;  
        break;
      }
      distance = DISTANCEBASE[code_d];

       
      numextrabits_d = DISTANCEEXTRA[code_d];
      if(*bp >= inbitlength) ERROR_BREAK(51);  

      distance += readBitsFromStream(bp, in, numextrabits_d);

       
      start = (*pos);
      if(distance > start) ERROR_BREAK(52);  
      backward = start - distance;

      if(!ucvector_resize(out, (*pos) + length)) ERROR_BREAK(83  );
      for(forward = 0; forward < length; forward++)
      {
        out->data[(*pos)] = out->data[backward];
        (*pos)++;
        backward++;
        if(backward >= start) backward = start - distance;
      }
    }
    else if(code_ll == 256)
    {
      break;  
    }
    else    
    {
       
      error = (*bp) > inlength * 8 ? 10 : 11;
      break;
    }
  }

  HuffmanTree_cleanup(&tree_ll);
  HuffmanTree_cleanup(&tree_d);

  return error;
}
