 long long mkvparser::ReadUInt(IMkvReader* pReader, long long pos, long& len) {
  assert(pReader);
  assert(pos >= 0);
  int status;
// long long ReadUInt(IMkvReader* pReader, long long pos, long& len) {
//   if (!pReader || pos < 0)
//     return E_FILE_FORMAT_INVALID;
  
    len = 1;
    unsigned char b;
  status = pReader->Read(pos, 1, &b);
//   int status = pReader->Read(pos, 1, &b);
  
    if (status < 0)   
      return status;
 
  if (status > 0)  
  return E_BUFFER_NOT_FULL;
 
  if (b == 0)  
  return E_FILE_FORMAT_INVALID;
 
  unsigned char m = 0x80;
 
  while (!(b & m)) {
     m >>= 1;
 
      ++len;
    }
  
    long long result = b & (~m);
    ++pos;
  
  for (int i = 1; i < len; ++i) {
     status = pReader->Read(pos, 1, &b);
 
  if (status < 0) {
       len = 1;
  return status;
  }
 
  if (status > 0) {
       len = 1;
  return E_BUFFER_NOT_FULL;
  }
 
     result <<= 8;
     result |= b;
 
  ++pos;
  }
 
 
    return result;
  }