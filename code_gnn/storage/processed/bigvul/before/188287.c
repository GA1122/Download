 long Segment::DoLoadClusterUnknownSize(
    long long& pos,
    long& len)
{
    assert(m_pos < 0);
    assert(m_pUnknownSize);
// long Segment::DoLoadClusterUnknownSize(long long& pos, long& len) {
//   assert(m_pos < 0);
//   assert(m_pUnknownSize);
  
  #if 0
      assert(m_pUnknownSize->GetElementSize() < 0);   
 
  const long long element_start = m_pUnknownSize->m_element_start;
 
     pos = -m_pos;
     assert(pos > element_start);
 
 
  long long total, avail;
 
  long status = m_pReader->Length(&total, &avail);
 
  if (status < 0)  
  return status;
 
     assert((total < 0) || (avail <= total));
 
  const long long segment_stop = (m_size < 0) ? -1 : m_start + m_size;
 
  
      long long element_size = -1;
  
    for (;;)   
    {
//     for (;;) {   
          if ((total >= 0) && (pos >= total))
          {
              element_size = total - element_start;
             assert(element_size > 0);
 
  break;
  }
 
  if ((segment_stop >= 0) && (pos >= segment_stop))
  {
             element_size = segment_stop - element_start;
             assert(element_size > 0);
 
  break;
  }
 
 
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
 
 
  
        if ((id == 0x0F43B675) || (id == 0x0C53BB6B))  
        {
//         if ((id == 0x0F43B675) || (id == 0x0C53BB6B)) {   
              element_size = pos - element_start;
              assert(element_size > 0);
  
  break;
  }
 
 #ifdef _DEBUG
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
 #endif
 
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
 
     assert(element_size >= 0);
 
     m_pos = element_start + element_size;
     m_pUnknownSize = 0;
 
  
      return 2;   
  #else
    const long status = m_pUnknownSize->Parse(pos, len);
//   const long status = m_pUnknownSize->Parse(pos, len);
  
    if (status < 0)   
        return status;
//   if (status < 0)   
//     return status;
  
    if (status == 0)   
        return 2;      
//   if (status == 0)   
//     return 2;   
  
    assert(status > 0);    
//   assert(status > 0);   
  
    const long long start = m_pUnknownSize->m_element_start;
//   const long long start = m_pUnknownSize->m_element_start;
  
    const long long size = m_pUnknownSize->GetElementSize();
    assert(size >= 0);
//   const long long size = m_pUnknownSize->GetElementSize();
//   assert(size >= 0);
  
    pos = start + size;
    m_pos = pos;
//   pos = start + size;
//   m_pos = pos;
  
    m_pUnknownSize = 0;
//   m_pUnknownSize = 0;
  
    return 2;   
//   return 2;   
  #endif
  }