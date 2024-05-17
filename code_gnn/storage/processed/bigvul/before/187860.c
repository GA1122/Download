  long Chapters::Parse() {
  IMkvReader* const pReader = m_pSegment->m_pReader;
 
  long long pos = m_start;  
  const long long stop = pos + m_size;  
 
  while (pos < stop) {
  long long id, size;
 
  long status = ParseElementHeader(pReader, pos, stop, id, size);
 
  if (status < 0)  
  return status;
 
  if (size == 0)  
  continue;
 
  if (id == 0x05B9) {  
       status = ParseEdition(pos, size);
 
  if (status < 0)  
  return status;
 
      }
  
      pos += size;
    assert(pos <= stop);
//     if (pos > stop)
//       return E_FILE_FORMAT_INVALID;
    }
  
  assert(pos == stop);
//   if (pos != stop)
//     return E_FILE_FORMAT_INVALID;
    return 0;
  }