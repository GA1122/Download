 long long mkvparser::GetUIntLength(IMkvReader* pReader, long long pos,
                                   long& len) {
  assert(pReader);
  assert(pos >= 0);
// long long ReadID(IMkvReader* pReader, long long pos, long& len) {
//   const long long id = ReadUInt(pReader, pos, len);
//   if (id < 0 || len < 1 || len > 4) {
//      
//      
//     return E_FILE_FORMAT_INVALID;
//   }
//   return id;
// }
// 
// long long GetUIntLength(IMkvReader* pReader, long long pos, long& len) {
//   if (!pReader || pos < 0)
//     return E_FILE_FORMAT_INVALID;
  
    long long total, available;
  
    int status = pReader->Length(&total, &available);
  assert(status >= 0);
  assert((total < 0) || (available <= total));
//   if (status < 0 || (total >= 0 && available > total))
//     return E_FILE_FORMAT_INVALID;
  
    len = 1;
  
  if (pos >= available)
  return pos;  
 
  unsigned char b;
 
  
    status = pReader->Read(pos, 1, &b);
  
  if (status < 0)
//   if (status != 0)
      return status;
  
  assert(status == 0);
    if (b == 0)   
      return E_FILE_FORMAT_INVALID;
  
  unsigned char m = 0x80;
 
  while (!(b & m)) {
     m >>= 1;
  ++len;
  }
 
  return 0;  
 }