 void CuePoint::TrackPosition::Parse(IMkvReader* pReader, long long start_,
// bool CuePoint::TrackPosition::Parse(IMkvReader* pReader, long long start_,
                                      long long size_) {
    const long long stop = start_ + size_;
    long long pos = start_;
 
   m_track = -1;
   m_pos = -1;
   m_block = 1;  
 
 
    while (pos < stop) {
      long len;
  
    const long long id = ReadUInt(pReader, pos, len);
    assert(id >= 0);   
    assert((pos + len) <= stop);
//     const long long id = ReadID(pReader, pos, len);
//     if ((id < 0) || ((pos + len) > stop)) {
//       return false;
//     }
  
      pos += len;   
  
      const long long size = ReadUInt(pReader, pos, len);
    assert(size >= 0);
    assert((pos + len) <= stop);
//     if ((size < 0) || ((pos + len) > stop)) {
//       return false;
//     }
  
      pos += len;   
    assert((pos + size) <= stop);
//     if ((pos + size) > stop) {
//       return false;
//     }
  
      if (id == 0x77)   
        m_track = UnserializeUInt(pReader, pos, size);
 
  else if (id == 0x71)  
       m_pos = UnserializeUInt(pReader, pos, size);
 
  else if (id == 0x1378)  
 
        m_block = UnserializeUInt(pReader, pos, size);
  
      pos += size;   
    assert(pos <= stop);
    }
  
  assert(m_pos >= 0);
  assert(m_track > 0);
//   if ((m_pos < 0) || (m_track <= 0)) {
//     return false;
//   }
// 
//   return true;
  }