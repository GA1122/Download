 long Segment::DoLoadCluster(long long& pos, long& len) {
  if (m_pos < 0)
  return DoLoadClusterUnknownSize(pos, len);
 
  long long total, avail;
 
  long status = m_pReader->Length(&total, &avail);
 
 
    if (status < 0)   
      return status;
  
  assert((total < 0) || (avail <= total));
//   if (total >= 0 && avail > total)
//     return E_FILE_FORMAT_INVALID;
  
    const long long segment_stop = (m_size < 0) ? -1 : m_start + m_size;
  
  long long cluster_off = -1;  
  long long cluster_size = -1;  
 
  for (;;) {
  if ((total >= 0) && (m_pos >= total))
  return 1;  
 
  if ((segment_stop >= 0) && (m_pos >= segment_stop))
  return 1;  
 
     pos = m_pos;
 
 
  if ((pos + 1) > avail) {
       len = 1;
  return E_BUFFER_NOT_FULL;
  }
 
  long long result = GetUIntLength(m_pReader, pos, len);
 
  if (result < 0)  
  return static_cast<long>(result);
 
  if (result > 0)  
  return E_BUFFER_NOT_FULL;
 
  if ((segment_stop >= 0) && ((pos + len) > segment_stop))
  return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > avail)
 
        return E_BUFFER_NOT_FULL;
  
      const long long idpos = pos;
    const long long id = ReadUInt(m_pReader, idpos, len);
//     const long long id = ReadID(m_pReader, idpos, len);
  
    if (id < 0)   
      return static_cast<long>(id);
//     if (id < 0)
//       return E_FILE_FORMAT_INVALID;
  
      pos += len;   
  
 
  if ((pos + 1) > avail) {
       len = 1;
  return E_BUFFER_NOT_FULL;
  }
 
     result = GetUIntLength(m_pReader, pos, len);
 
  if (result < 0)  
  return static_cast<long>(result);
 
  if (result > 0)  
  return E_BUFFER_NOT_FULL;
 
  if ((segment_stop >= 0) && ((pos + len) > segment_stop))
  return E_FILE_FORMAT_INVALID;
 
  if ((pos + len) > avail)
  return E_BUFFER_NOT_FULL;
 
  const long long size = ReadUInt(m_pReader, pos, len);
 
  if (size < 0)  
  return static_cast<long>(size);
 
     pos += len;  
 
 
  if (size == 0) {  
       m_pos = pos;
  continue;
  }
 
  
      const long long unknown_size = (1LL << (7 * len)) - 1;
  
#if 0   
        if (size == unknown_size)
            return E_FILE_FORMAT_INVALID;   
#endif
      if ((segment_stop >= 0) && (size != unknown_size) &&
          ((pos + size) > segment_stop)) {
        return E_FILE_FORMAT_INVALID;
      }
  
#if 0   
        len = static_cast<long>(size);
        if ((pos + size) > avail)
            return E_BUFFER_NOT_FULL;
#endif
      if (id == 0x0C53BB6B) {   
        if (size == unknown_size)
          return E_FILE_FORMAT_INVALID;   
 
 
        if (m_pCues == NULL) {
          const long long element_size = (pos - idpos) + size;
  
        m_pCues = new Cues(this, pos, size, idpos, element_size);
        assert(m_pCues);   
//         m_pCues = new (std::nothrow) Cues(this, pos, size, idpos, element_size);
//         if (m_pCues == NULL)
//           return -1;
        }
  
        m_pos = pos + size;   
  continue;
  }
 
  if (id != 0x0F43B675) {  
  if (size == unknown_size)
  return E_FILE_FORMAT_INVALID;  
 
       m_pos = pos + size;  
  continue;
  }
 
 
     cluster_off = idpos - m_start;  
 
  if (size != unknown_size)
       cluster_size = size;
 
 
      break;
    }
  
  assert(cluster_off >= 0);   
//   if (cluster_off < 0) {
//      
//     return E_FILE_FORMAT_INVALID;
//   }
  
    long long pos_;
    long len_;
 
   status = Cluster::HasBlockEntries(this, cluster_off, pos_, len_);
 
  if (status < 0) {  
     pos = pos_;
     len = len_;
 
  return status;
  }
 
 
 
 
    const long idx = m_clusterCount;
  
    if (m_clusterPreloadCount > 0) {
    assert(idx < m_clusterSize);
//     if (idx >= m_clusterSize)
//       return E_FILE_FORMAT_INVALID;
  
      Cluster* const pCluster = m_clusters[idx];
    assert(pCluster);
    assert(pCluster->m_index < 0);
//     if (pCluster == NULL || pCluster->m_index >= 0)
//       return E_FILE_FORMAT_INVALID;
  
      const long long off = pCluster->GetPosition();
    assert(off >= 0);
//     if (off < 0)
//       return E_FILE_FORMAT_INVALID;
  
      if (off == cluster_off) {   
        if (status == 0)   
  return E_FILE_FORMAT_INVALID;
 
  if (cluster_size >= 0)
         pos += cluster_size;
  else {
  const long long element_size = pCluster->GetElementSize();
 
  if (element_size <= 0)
  return E_FILE_FORMAT_INVALID;  
 
         pos = pCluster->m_element_start + element_size;
  }
 
       pCluster->m_index = idx;  
  ++m_clusterCount;
 
        --m_clusterPreloadCount;
  
        m_pos = pos;   
      assert((segment_stop < 0) || (m_pos <= segment_stop));
//       if (segment_stop >= 0 && m_pos > segment_stop)
//         return E_FILE_FORMAT_INVALID;
  
        return 0;   
      }
    }
  
    if (status == 0) {   
    if (cluster_size < 0)
      return E_FILE_FORMAT_INVALID;   
    pos += cluster_size;
//     if (cluster_size >= 0)
//       pos += cluster_size;
  
      if ((total >= 0) && (pos >= total)) {
        m_pos = total;
  return 1;  
  }
 
  if ((segment_stop >= 0) && (pos >= segment_stop)) {
       m_pos = segment_stop;
  return 1;  
  }
 
     m_pos = pos;
  return 2;  
  }
 
 
  
    Cluster* const pCluster = Cluster::Create(this, idx, cluster_off);
  assert(pCluster);
//   if (pCluster == NULL)
//     return -1;
  
  AppendCluster(pCluster);
  assert(m_clusters);
  assert(idx < m_clusterSize);
  assert(m_clusters[idx] == pCluster);
//   if (!AppendCluster(pCluster)) {
//     delete pCluster;
//     return -1;
//   }
  
    if (cluster_size >= 0) {
      pos += cluster_size;
  
      m_pos = pos;
    assert((segment_stop < 0) || (m_pos <= segment_stop));
// 
//     if (segment_stop > 0 && m_pos > segment_stop)
//       return E_FILE_FORMAT_INVALID;
  
      return 0;
    }
 
   m_pUnknownSize = pCluster;
   m_pos = -pos;
 
  
    return 0;   
  
#if 0
    if (cluster_size < 0) {   
        const long long payload_pos = pos;   
        for (;;) {   
            if ((total >= 0) && (pos >= total))
                break;
            if ((segment_stop >= 0) && (pos >= segment_stop))
                break;   
            if ((pos + 1) > avail)
            {
                len = 1;
                return E_BUFFER_NOT_FULL;
            }
            long long result = GetUIntLength(m_pReader, pos, len);
            if (result < 0)   
                return static_cast<long>(result);
            if (result > 0)   
                return E_BUFFER_NOT_FULL;
            if ((segment_stop >= 0) && ((pos + len) > segment_stop))
                return E_FILE_FORMAT_INVALID;
            if ((pos + len) > avail)
                return E_BUFFER_NOT_FULL;
            const long long idpos = pos;
            const long long id = ReadUInt(m_pReader, idpos, len);
            if (id < 0)   
                return static_cast<long>(id);
            if (id == 0x0F43B675)   
                break;
            if (id == 0x0C53BB6B)   
                break;
            switch (id)
            {
                case 0x20:   
                case 0x23:   
                case 0x67:   
                case 0x2B:   
                    break;
                default:
                    assert(false);
                    break;
            }
            pos += len;   
            if ((pos + 1) > avail)
            {
                len = 1;
                return E_BUFFER_NOT_FULL;
            }
            result = GetUIntLength(m_pReader, pos, len);
            if (result < 0)   
                return static_cast<long>(result);
            if (result > 0)   
                return E_BUFFER_NOT_FULL;
            if ((segment_stop >= 0) && ((pos + len) > segment_stop))
                return E_FILE_FORMAT_INVALID;
            if ((pos + len) > avail)
                return E_BUFFER_NOT_FULL;
            const long long size = ReadUInt(m_pReader, pos, len);
            if (size < 0)   
                return static_cast<long>(size);
            pos += len;   
            if (size == 0)   
                continue;
            const long long unknown_size = (1LL << (7 * len)) - 1;
            if (size == unknown_size)
                return E_FILE_FORMAT_INVALID;   
            if ((segment_stop >= 0) && ((pos + size) > segment_stop))   
                return E_FILE_FORMAT_INVALID;
            pos += size;   
            assert((segment_stop < 0) || (pos <= segment_stop));
        }   
        cluster_size = pos - payload_pos;
        assert(cluster_size >= 0);
        pos = payload_pos;   
    }
    if (m_clusterPreloadCount > 0)
    {
        assert(idx < m_clusterSize);
        Cluster* const pCluster = m_clusters[idx];
        assert(pCluster);
        assert(pCluster->m_index < 0);
        const long long off = pCluster->GetPosition();
        assert(off >= 0);
        if (off == cluster_off)   
            return E_FILE_FORMAT_INVALID;   
    }
    m_pos = pos + cluster_size;   
    assert((segment_stop < 0) || (m_pos <= segment_stop));
    return 2;      
#endif
//    
//    
//    
  }