static int formatIPTCfromBuffer(Image *ofile, char *s, ssize_t len)
{
  char
    temp[MagickPathExtent];

  unsigned int
    foundiptc,
    tagsfound;

  unsigned char
    recnum,
    dataset;

  unsigned char
    *readable,
    *str;

  ssize_t
    tagindx,
    taglen;

  int
    i,
    tagcount = (int) (sizeof(tags) / sizeof(tag_spec));

  int
    c;

  foundiptc = 0;  
  tagsfound = 0;  

  while (len > 0)
  {
    c = *s++; len--;
    if (c == 0x1c)
      foundiptc = 1;
    else
      {
        if (foundiptc)
          return -1;
        else
          continue;
      }
     
    c = *s++; len--;
    if (len < 0) return -1;
    dataset = (unsigned char) c;
    c = *s++; len--;
    if (len < 0) return -1;
    recnum = (unsigned char) c;
     
    for (i=0; i< tagcount; i++)
      if (tags[i].id == (short) recnum)
        break;
    if (i < tagcount)
      readable=(unsigned char *) tags[i].name;
    else
      readable=(unsigned char *) "";
     
    c=(*s++);
    len--;
    if (len < 0)
      return(-1);
    if (c & (unsigned char) 0x80)
      return(0);
    else
      {
        s--;
        len++;
        taglen=readWordFromBuffer(&s, &len);
      }
    if (taglen < 0)
      return(-1);
    if (taglen > 65535)
      return(-1);
     
    str=(unsigned char *) AcquireQuantumMemory((size_t) (taglen+MagickPathExtent),
      sizeof(*str));
    if (str == (unsigned char *) NULL)
      printf("MemoryAllocationFailed");
    for (tagindx=0; tagindx<taglen; tagindx++)
    {
      c = *s++; len--;
      if (len < 0)
        return(-1);
      str[tagindx]=(unsigned char) c;
    }
    str[taglen]=0;

     
    if (strlen((char *)readable) > 0)
      (void) FormatLocaleString(temp,MagickPathExtent,"%d#%d#%s=",
        (unsigned int) dataset,(unsigned int) recnum, readable);
    else
      (void) FormatLocaleString(temp,MagickPathExtent,"%d#%d=",
        (unsigned int) dataset,(unsigned int) recnum);
    (void) WriteBlobString(ofile,temp);
    formatString( ofile, (char *)str, taglen );
    str=(unsigned char *) RelinquishMagickMemory(str);

    tagsfound++;
  }
  return ((int) tagsfound);
}
