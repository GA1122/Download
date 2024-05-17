static unsigned encodeLZ77(uivector* out, Hash* hash,
                           const unsigned char* in, size_t inpos, size_t insize, unsigned windowsize,
                           unsigned minmatch, unsigned nicematch, unsigned lazymatching)
{
  size_t pos;
  unsigned i, error = 0;
   
  unsigned maxchainlength = windowsize >= 8192 ? windowsize : windowsize / 8;
  unsigned maxlazymatch = windowsize >= 8192 ? MAX_SUPPORTED_DEFLATE_LENGTH : 64;

  unsigned usezeros = 1;  
  unsigned numzeros = 0;

  unsigned offset;  
  unsigned length;
  unsigned lazy = 0;
  unsigned lazylength = 0, lazyoffset = 0;
  unsigned hashval;
  unsigned current_offset, current_length;
  unsigned prev_offset;
  const unsigned char *lastptr, *foreptr, *backptr;
  unsigned hashpos;

  if(windowsize <= 0 || windowsize > 32768) return 60;  
  if((windowsize & (windowsize - 1)) != 0) return 90;  

  if(nicematch > MAX_SUPPORTED_DEFLATE_LENGTH) nicematch = MAX_SUPPORTED_DEFLATE_LENGTH;

  for(pos = inpos; pos < insize; pos++)
  {
    size_t wpos = pos & (windowsize - 1);  
    unsigned chainlength = 0;

    hashval = getHash(in, insize, pos);

    if(usezeros && hashval == 0)
    {
      if (numzeros == 0) numzeros = countZeros(in, insize, pos);
      else if (pos + numzeros > insize || in[pos + numzeros - 1] != 0) numzeros--;
    }
    else
    {
      numzeros = 0;
    }

    updateHashChain(hash, wpos, hashval, numzeros);

     
    length = 0;
    offset = 0;

    hashpos = hash->chain[wpos];

    lastptr = &in[insize < pos + MAX_SUPPORTED_DEFLATE_LENGTH ? insize : pos + MAX_SUPPORTED_DEFLATE_LENGTH];

     
    prev_offset = 0;
    for(;;)
    {
      if(chainlength++ >= maxchainlength) break;
      current_offset = hashpos <= wpos ? wpos - hashpos : wpos - hashpos + windowsize;

      if(current_offset < prev_offset) break;  
      prev_offset = current_offset;
      if(current_offset > 0)
      {
         
        foreptr = &in[pos];
        backptr = &in[pos - current_offset];

         
        if(numzeros >= 3)
        {
          unsigned skip = hash->zeros[hashpos];
          if(skip > numzeros) skip = numzeros;
          backptr += skip;
          foreptr += skip;
        }

        while(foreptr != lastptr && *backptr == *foreptr)  
        {
          ++backptr;
          ++foreptr;
        }
        current_length = (unsigned)(foreptr - &in[pos]);

        if(current_length > length)
        {
          length = current_length;  
          offset = current_offset;  
           
          if(current_length >= nicematch) break;
        }
      }

      if(hashpos == hash->chain[hashpos]) break;
      
      if(numzeros >= 3 && length > numzeros) {
        hashpos = hash->chainz[hashpos];
        if(hash->zeros[hashpos] != numzeros) break;
      } else {
        hashpos = hash->chain[hashpos];
         
        if(hash->val[hashpos] != (int)hashval) break;
      }
    }

    if(lazymatching)
    {
      if(!lazy && length >= 3 && length <= maxlazymatch && length < MAX_SUPPORTED_DEFLATE_LENGTH)
      {
        lazy = 1;
        lazylength = length;
        lazyoffset = offset;
        continue;  
      }
      if(lazy)
      {
        lazy = 0;
        if(pos == 0) ERROR_BREAK(81);
        if(length > lazylength + 1)
        {
           
          if(!uivector_push_back(out, in[pos - 1])) ERROR_BREAK(83  );
        }
        else
        {
          length = lazylength;
          offset = lazyoffset;
          hash->head[hashval] = -1;  
          hash->headz[numzeros] = -1;  
          pos--;
        }
      }
    }
    if(length >= 3 && offset > windowsize) ERROR_BREAK(86  );

     
    if(length < 3)  
    {
      if(!uivector_push_back(out, in[pos])) ERROR_BREAK(83  );
    }
    else if(length < minmatch || (length == 3 && offset > 4096))
    {
       
      if(!uivector_push_back(out, in[pos])) ERROR_BREAK(83  );
    }
    else
    {
      addLengthDistance(out, length, offset);
      for(i = 1; i < length; i++)
      {
        pos++;
        wpos = pos & (windowsize - 1);
        hashval = getHash(in, insize, pos);
        if(usezeros && hashval == 0)
        {
          if (numzeros == 0) numzeros = countZeros(in, insize, pos);
          else if (pos + numzeros > insize || in[pos + numzeros - 1] != 0) numzeros--;
        }
        else
        {
          numzeros = 0;
        }
        updateHashChain(hash, wpos, hashval, numzeros);
      }
    }
  }  

  return error;
}
