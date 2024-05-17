  bool SeekHead::ParseEntry(IMkvReader* pReader, long long start, long long size_,
                            Entry* pEntry) {
    if (size_ <= 0)
  return false;
 
  long long pos = start;
  const long long stop = start + size_;
 
  long len;
 
  
  
  const long long seekIdId = ReadUInt(pReader, pos, len);
//   const long long seekIdId = ReadID(pReader, pos, len);
//   if (seekIdId < 0)
//     return false;
  
    if (seekIdId != 0x13AB)   
      return false;
 
  if ((pos + len) > stop)
  return false;
 
   pos += len;  
 
  const long long seekIdSize = ReadUInt(pReader, pos, len);
 
  if (seekIdSize <= 0)
  return false;
 
  if ((pos + len) > stop)
  return false;
 
   pos += len;  
 
  if ((pos + seekIdSize) > stop)
  return false;
 
 
   pEntry->id = ReadUInt(pReader, pos, len);  
 
  if (pEntry->id <= 0)
  return false;
 
  if (len != seekIdSize)
  return false;
 
   pos += seekIdSize;  
 
  const long long seekPosId = ReadUInt(pReader, pos, len);
 
  if (seekPosId != 0x13AC)  
  return false;
 
  if ((pos + len) > stop)
  return false;
 
   pos += len;  
 
  const long long seekPosSize = ReadUInt(pReader, pos, len);
 
  if (seekPosSize <= 0)
  return false;
 
  if ((pos + len) > stop)
  return false;
 
   pos += len;  
 
  if ((pos + seekPosSize) > stop)
  return false;
 
   pEntry->pos = UnserializeUInt(pReader, pos, seekPosSize);
 
  if (pEntry->pos < 0)
  return false;
 
   pos += seekPosSize;  
 
  if (pos != stop)
  return false;
 
  
    return true;
  }