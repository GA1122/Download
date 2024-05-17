  bool Cues::LoadCuePoint() const {
    const long long stop = m_start + m_size;
  
    if (m_pos >= stop)
      return false;   
  
  Init();
//   if (!Init()) {
//     m_pos = stop;
//     return false;
//   }
  
    IMkvReader* const pReader = m_pSegment->m_pReader;
  
  while (m_pos < stop) {
  const long long idpos = m_pos;
 
  
      long len;
  
    const long long id = ReadUInt(pReader, m_pos, len);
    assert(id >= 0);   
    assert((m_pos + len) <= stop);
//     const long long id = ReadID(pReader, m_pos, len);
//     if (id < 0 || (m_pos + len) > stop)
//       return false;
  
      m_pos += len;   
  
      const long long size = ReadUInt(pReader, m_pos, len);
    assert(size >= 0);
    assert((m_pos + len) <= stop);
//     if (size < 0 || (m_pos + len) > stop)
//       return false;
  
      m_pos += len;   
    assert((m_pos + size) <= stop);
//     if ((m_pos + size) > stop)
//       return false;
  
      if (id != 0x3B) {   
        m_pos += size;   
      assert(m_pos <= stop);
//       if (m_pos > stop)
//         return false;
  
        continue;
      }
  
    assert(m_preload_count > 0);
    CuePoint* const pCP = m_cue_points[m_count];
    assert(pCP);
    assert((pCP->GetTimeCode() >= 0) || (-pCP->GetTimeCode() == idpos));
    if (pCP->GetTimeCode() < 0 && (-pCP->GetTimeCode() != idpos))
//     if (m_preload_count < 1)
        return false;
  
    pCP->Load(pReader);
//     CuePoint* const pCP = m_cue_points[m_count];
//     if (!pCP || (pCP->GetTimeCode() < 0 && (-pCP->GetTimeCode() != idpos)))
//       return false;
// 
//     if (!pCP->Load(pReader)) {
//       m_pos = stop;
//       return false;
//     }
      ++m_count;
      --m_preload_count;
  
      m_pos += size;   
    assert(m_pos <= stop);
//     if (m_pos > stop)
//       return false;
  
      return true;   
    }
  
  }