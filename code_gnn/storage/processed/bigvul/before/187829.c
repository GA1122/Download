  long long Segment::CreateInstance(IMkvReader* pReader, long long pos,
                                    Segment*& pSegment) {
  assert(pReader);
  assert(pos >= 0);
//   if (pReader == NULL || pos < 0)
//     return E_PARSE_FAILED;
  
    pSegment = NULL;
  
  long long total, available;
 
  const long status = pReader->Length(&total, &available);
 
  if (status < 0)  
  return status;
 
  if (available < 0)
  return -1;
 
  if ((total >= 0) && (available > total))
  return -1;
 
 
  for (;;) {
  if ((total >= 0) && (pos >= total))
  return E_FILE_FORMAT_INVALID;
 
  long len;
  long long result = GetUIntLength(pReader, pos, len);
 
  if (result)  
  return result;
 
  if ((total >= 0) && ((pos + len) > total))
  return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > available)
 
        return pos + len;
  
      const long long idpos = pos;
    const long long id = ReadUInt(pReader, pos, len);
//     const long long id = ReadID(pReader, pos, len);
  
    if (id < 0)   
      return id;
//     if (id < 0)
//       return E_FILE_FORMAT_INVALID;
  
      pos += len;   
  
 
     result = GetUIntLength(pReader, pos, len);
 
  if (result)  
  return result;
 
  if ((total >= 0) && ((pos + len) > total))
  return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > available)
  return pos + len;
 
  long long size = ReadUInt(pReader, pos, len);
 
  if (size < 0)  
  return size;
 
     pos += len;  
 
 
  const long long unknown_size = (1LL << (7 * len)) - 1;
 
  if (id == 0x08538067) {  
  if (size == unknown_size)
         size = -1;
 
  else if (total < 0)
         size = -1;
 
  else if ((pos + size) > total)
         size = -1;
 
       pSegment = new (std::nothrow) Segment(pReader, idpos,
                                             pos, size);
 
  if (pSegment == 0)
  return -1;  
 
  return 0;  
  }
 
  if (size == unknown_size)
  return E_FILE_FORMAT_INVALID;
 
  if ((total >= 0) && ((pos + size) > total))
  return E_FILE_FORMAT_INVALID;
 
  if ((pos + size) > available)
  return pos + size;
 
     pos += size;  
  }
 }