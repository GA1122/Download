 long SegmentInfo::Parse() {
   assert(m_pMuxingAppAsUTF8 == NULL);
   assert(m_pWritingAppAsUTF8 == NULL);
   assert(m_pTitleAsUTF8 == NULL);
 
  IMkvReader* const pReader = m_pSegment->m_pReader;
 
  long long pos = m_start;
  const long long stop = m_start + m_size;
 
   m_timecodeScale = 1000000;
   m_duration = -1;
 
  while (pos < stop) {
  long long id, size;
 
  const long status = ParseElementHeader(pReader, pos, stop, id, size);
 
  if (status < 0)  
  return status;
 
  if (id == 0x0AD7B1) {  
       m_timecodeScale = UnserializeUInt(pReader, pos, size);
 
  if (m_timecodeScale <= 0)
  return E_FILE_FORMAT_INVALID;
  } else if (id == 0x0489) {  
  const long status = UnserializeFloat(pReader, pos, size, m_duration);
 
  if (status < 0)
  return status;
 
  if (m_duration < 0)
  return E_FILE_FORMAT_INVALID;
  } else if (id == 0x0D80) {  
  const long status =
  UnserializeString(pReader, pos, size, m_pMuxingAppAsUTF8);
 
  if (status)
  return status;
  } else if (id == 0x1741) {  
  const long status =
  UnserializeString(pReader, pos, size, m_pWritingAppAsUTF8);
 
  if (status)
  return status;
  } else if (id == 0x3BA9) {  
  const long status = UnserializeString(pReader, pos, size, m_pTitleAsUTF8);
 
  if (status)
  return status;
 
      }
  
      pos += size;
    assert(pos <= stop);
// 
//     if (pos > stop)
//       return E_FILE_FORMAT_INVALID;
    }
  
  assert(pos == stop);
//   const double rollover_check = m_duration * m_timecodeScale;
//   if (rollover_check > LONG_LONG_MAX)
//     return E_FILE_FORMAT_INVALID;
// 
//   if (pos != stop)
//     return E_FILE_FORMAT_INVALID;
  
    return 0;
  }