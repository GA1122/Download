 long long Segment::ParseHeaders() {
  long long total, available;
 
  const int status = m_pReader->Length(&total, &available);
 
 
    if (status < 0)   
      return status;
  
  assert((total < 0) || (available <= total));
//   if (total > 0 && available > total)
//     return E_FILE_FORMAT_INVALID;
  
    const long long segment_stop = (m_size < 0) ? -1 : m_start + m_size;
  assert((segment_stop < 0) || (total < 0) || (segment_stop <= total));
  assert((segment_stop < 0) || (m_pos <= segment_stop));
// 
//   if ((segment_stop >= 0 && total >= 0 && segment_stop > total) ||
//       (segment_stop >= 0 && m_pos > segment_stop)) {
//     return E_FILE_FORMAT_INVALID;
//   }
  
    for (;;) {
      if ((total >= 0) && (m_pos >= total))
  break;
 
  if ((segment_stop >= 0) && (m_pos >= segment_stop))
  break;
 
 
      long long pos = m_pos;
      const long long element_start = pos;
  
//      
//     unsigned long long rollover_check = pos + 1ULL;
//     if (rollover_check > LONG_LONG_MAX)
//       return E_FILE_FORMAT_INVALID;
// 
      if ((pos + 1) > available)
        return (pos + 1);
  
  long len;
  long long result = GetUIntLength(m_pReader, pos, len);
 
 
      if (result < 0)   
        return result;
  
    if (result > 0)   
//     if (result > 0) {
//        
        return (pos + 1);
//     }
  
      if ((segment_stop >= 0) && ((pos + len) > segment_stop))
        return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > available)
 
        return pos + len;
  
      const long long idpos = pos;
    const long long id = ReadUInt(m_pReader, idpos, len);
//     const long long id = ReadID(m_pReader, idpos, len);
  
    if (id < 0)   
      return id;
//     if (id < 0)
//       return E_FILE_FORMAT_INVALID;
  
      if (id == 0x0F43B675)   
        break;
 
     pos += len;  
 
  if ((pos + 1) > available)
  return (pos + 1);
 
     result = GetUIntLength(m_pReader, pos, len);
 
 
      if (result < 0)   
        return result;
  
    if (result > 0)   
//     if (result > 0) {
//        
        return (pos + 1);
//     }
  
      if ((segment_stop >= 0) && ((pos + len) > segment_stop))
        return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > available)
  return pos + len;
 
  
      const long long size = ReadUInt(m_pReader, pos, len);
  
    if (size < 0)   
//     if (size < 0 || len < 1 || len > 8) {
//        
//        
        return size;
//     }
  
      pos += len;   
  
//      
//     rollover_check = static_cast<unsigned long long>(pos) + size;
//     if (rollover_check > LONG_LONG_MAX)
//       return E_FILE_FORMAT_INVALID;
// 
      const long long element_size = size + pos - element_start;
  
 
  if ((segment_stop >= 0) && ((pos + size) > segment_stop))
  return E_FILE_FORMAT_INVALID;
 
 
  if ((pos + size) > available)
  return pos + size;
 
  if (id == 0x0549A966) {  
  if (m_pInfo)
  return E_FILE_FORMAT_INVALID;
 
       m_pInfo = new (std::nothrow)
  SegmentInfo(this, pos, size, element_start, element_size);
 
  if (m_pInfo == NULL)
  return -1;
 
  const long status = m_pInfo->Parse();
 
  if (status)
  return status;
  } else if (id == 0x0654AE6B) {  
  if (m_pTracks)
  return E_FILE_FORMAT_INVALID;
 
       m_pTracks = new (std::nothrow)
  Tracks(this, pos, size, element_start, element_size);
 
  if (m_pTracks == NULL)
  return -1;
 
  const long status = m_pTracks->Parse();
 
  if (status)
  return status;
  } else if (id == 0x0C53BB6B) {  
  if (m_pCues == NULL) {
         m_pCues = new (std::nothrow)
  Cues(this, pos, size, element_start, element_size);
 
  if (m_pCues == NULL)
  return -1;
  }
  } else if (id == 0x014D9B74) {  
  if (m_pSeekHead == NULL) {
         m_pSeekHead = new (std::nothrow)
  SeekHead(this, pos, size, element_start, element_size);
 
  if (m_pSeekHead == NULL)
  return -1;
 
  const long status = m_pSeekHead->Parse();
 
  if (status)
  return status;
  }
  } else if (id == 0x0043A770) {  
  if (m_pChapters == NULL) {
         m_pChapters = new (std::nothrow)
  Chapters(this, pos, size, element_start, element_size);
 
  if (m_pChapters == NULL)
  return -1;
 
  const long status = m_pChapters->Parse();
 
 
          if (status)
            return status;
        }
//     } else if (id == 0x0254C367) {   
//       if (m_pTags == NULL) {
//         m_pTags = new (std::nothrow)
//             Tags(this, pos, size, element_start, element_size);
// 
//         if (m_pTags == NULL)
//           return -1;
// 
//         const long status = m_pTags->Parse();
// 
//         if (status)
//           return status;
//       }
      }
  
      m_pos = pos + size;   
    }
  
  assert((segment_stop < 0) || (m_pos <= segment_stop));
//   if (segment_stop >= 0 && m_pos > segment_stop)
//     return E_FILE_FORMAT_INVALID;
  
    if (m_pInfo == NULL)   
      return E_FILE_FORMAT_INVALID;
 
  if (m_pTracks == NULL)
  return E_FILE_FORMAT_INVALID;
 
  return 0;  
 }