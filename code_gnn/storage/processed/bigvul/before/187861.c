 long Chapters::Edition::Parse(IMkvReader* pReader, long long pos,
  long long size) {
  const long long stop = pos + size;
 
  while (pos < stop) {
  long long id, size;
 
  long status = ParseElementHeader(pReader, pos, stop, id, size);
 
  if (status < 0)  
  return status;
 
  if (size == 0)  
  continue;
 
  if (id == 0x36) {  
       status = ParseAtom(pReader, pos, size);
 
  if (status < 0)  
  return status;
 
      }
  
      pos += size;
    assert(pos <= stop);
//     if (pos > stop)
//       return E_FILE_FORMAT_INVALID;
    }
  
  assert(pos == stop);
//   if (pos != stop)
//     return E_FILE_FORMAT_INVALID;
    return 0;
  }