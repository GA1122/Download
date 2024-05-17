 void Cues::Init() const {
// bool Cues::Init() const {
    if (m_cue_points)
    return;
//     return true;
  
  assert(m_count == 0);
  assert(m_preload_count == 0);
//   if (m_count != 0 || m_preload_count != 0)
//     return false;
  
    IMkvReader* const pReader = m_pSegment->m_pReader;
  
  const long long stop = m_start + m_size;
  long long pos = m_start;
 
  long cue_points_size = 0;
 
  while (pos < stop) {
  const long long idpos = pos;
 
  
      long len;
  
    const long long id = ReadUInt(pReader, pos, len);
    assert(id >= 0);   
    assert((pos + len) <= stop);
//     const long long id = ReadID(pReader, pos, len);
//     if (id < 0 || (pos + len) > stop) {
//       return false;
//     }
  
      pos += len;   
  
      const long long size = ReadUInt(pReader, pos, len);
    assert(size >= 0);
    assert((pos + len) <= stop);
//     if (size < 0 || (pos + len > stop)) {
//       return false;
//     }
  
      pos += len;   
    assert((pos + size) <= stop);
//     if (pos + size > stop) {
//       return false;
//     }
  
    if (id == 0x3B)   
      PreloadCuePoint(cue_points_size, idpos);
//     if (id == 0x3B) {   
//       if (!PreloadCuePoint(cue_points_size, idpos))
//         return false;
//     }
  
     pos += size;   
    assert(pos <= stop);
    }
//   return true;
  }