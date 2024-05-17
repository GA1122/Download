  long Cluster::Load(long long& pos, long& len) const {
    assert(m_pSegment);
    assert(m_pos >= m_element_start);
 
  if (m_timecode >= 0)  
  return 0;
 
   assert(m_pos == m_element_start);
   assert(m_element_size < 0);
 
  IMkvReader* const pReader = m_pSegment->m_pReader;
 
  long long total, avail;
 
  const int status = pReader->Length(&total, &avail);
 
  if (status < 0)  
  return status;
 
   assert((total < 0) || (avail <= total));
   assert((total < 0) || (m_pos <= total));  
 
   pos = m_pos;
 
  long long cluster_size = -1;
 
  {
  if ((pos + 1) > avail) {
       len = 1;
  return E_BUFFER_NOT_FULL;
  }
 
  long long result = GetUIntLength(pReader, pos, len);
 
  if (result < 0)  
  return static_cast<long>(result);
 
  if (result > 0)  
  return E_BUFFER_NOT_FULL;
 
 
  if ((pos + len) > avail)
  return E_BUFFER_NOT_FULL;
 
  const long long id_ = ReadUInt(pReader, pos, len);
 
  if (id_ < 0)  
  return static_cast<long>(id_);
 
  if (id_ != 0x0F43B675)  
  return E_FILE_FORMAT_INVALID;
 
     pos += len;  
 
 
  if ((pos + 1) > avail) {
       len = 1;
  return E_BUFFER_NOT_FULL;
  }
 
     result = GetUIntLength(pReader, pos, len);
 
  if (result < 0)  
  return static_cast<long>(result);
 
  if (result > 0)  
  return E_BUFFER_NOT_FULL;
 
 
  if ((pos + len) > avail)
  return E_BUFFER_NOT_FULL;
 
  const long long size = ReadUInt(pReader, pos, len);
 
  if (size < 0)  
  return static_cast<long>(cluster_size);
 
  if (size == 0)
  return E_FILE_FORMAT_INVALID;  
 
     pos += len;  
 
  const long long unknown_size = (1LL << (7 * len)) - 1;
 
  if (size != unknown_size)
 
        cluster_size = size;
    }
  
#if 0
    len = static_cast<long>(size_);
    if (cluster_stop > avail)
        return E_BUFFER_NOT_FULL;
#endif
//    
    long long timecode = -1;
    long long new_pos = -1;
    bool bBlock = false;
 
  long long cluster_stop = (cluster_size < 0) ? -1 : pos + cluster_size;
 
  for (;;) {
  if ((cluster_stop >= 0) && (pos >= cluster_stop))
  break;
 
 
  if ((pos + 1) > avail) {
       len = 1;
  return E_BUFFER_NOT_FULL;
  }
 
  long long result = GetUIntLength(pReader, pos, len);
 
  if (result < 0)  
  return static_cast<long>(result);
 
  if (result > 0)  
  return E_BUFFER_NOT_FULL;
 
  if ((cluster_stop >= 0) && ((pos + len) > cluster_stop))
  return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > avail)
  return E_BUFFER_NOT_FULL;
 
  const long long id = ReadUInt(pReader, pos, len);
 
  if (id < 0)  
  return static_cast<long>(id);
 
  if (id == 0)
  return E_FILE_FORMAT_INVALID;
 
 
  if (id == 0x0F43B675)  
  break;
 
  if (id == 0x0C53BB6B)  
  break;
 
     pos += len;  
 
 
  if ((pos + 1) > avail) {
       len = 1;
  return E_BUFFER_NOT_FULL;
  }
 
     result = GetUIntLength(pReader, pos, len);
 
  if (result < 0)  
  return static_cast<long>(result);
 
  if (result > 0)  
  return E_BUFFER_NOT_FULL;
 
  if ((cluster_stop >= 0) && ((pos + len) > cluster_stop))
  return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > avail)
  return E_BUFFER_NOT_FULL;
 
  const long long size = ReadUInt(pReader, pos, len);
 
  if (size < 0)  
  return static_cast<long>(size);
 
  const long long unknown_size = (1LL << (7 * len)) - 1;
 
  if (size == unknown_size)
  return E_FILE_FORMAT_INVALID;
 
     pos += len;  
 
  if ((cluster_stop >= 0) && (pos > cluster_stop))
  return E_FILE_FORMAT_INVALID;
 
 
  if (size == 0)  
  continue;
 
  if ((cluster_stop >= 0) && ((pos + size) > cluster_stop))
  return E_FILE_FORMAT_INVALID;
 
  if (id == 0x67) {  
       len = static_cast<long>(size);
 
  if ((pos + size) > avail)
  return E_BUFFER_NOT_FULL;
 
       timecode = UnserializeUInt(pReader, pos, size);
 
  if (timecode < 0)  
  return static_cast<long>(timecode);
 
       new_pos = pos + size;
 
  if (bBlock)
  break;
  } else if (id == 0x20) {  
       bBlock = true;
  break;
  } else if (id == 0x23) {  
       bBlock = true;
  break;
 
      }
  
      pos += size;   
    assert((cluster_stop < 0) || (pos <= cluster_stop));
//     if (cluster_stop >= 0 && pos > cluster_stop)
//       return E_FILE_FORMAT_INVALID;
    }
  
  assert((cluster_stop < 0) || (pos <= cluster_stop));
//   if (cluster_stop >= 0 && pos > cluster_stop)
//     return E_FILE_FORMAT_INVALID;
  
    if (timecode < 0)   
      return E_FILE_FORMAT_INVALID;
 
  if (!bBlock)
  return E_FILE_FORMAT_INVALID;
 
   m_pos = new_pos;  
   m_timecode = timecode;  
 
  if (cluster_size >= 0)
     m_element_size = cluster_stop - m_element_start;
 
  return 0;
 }