 long Cluster::CreateBlockGroup(long long start_offset, long long size,
  long long discard_padding) {
   assert(m_entries);
   assert(m_entries_size > 0);
   assert(m_entries_count >= 0);
   assert(m_entries_count < m_entries_size);
 
  IMkvReader* const pReader = m_pSegment->m_pReader;
 
  long long pos = start_offset;
  const long long stop = start_offset + size;
 
 
  long long prev = 1;  
  long long next = 0;  
  long long duration = -1;  
 
  long long bpos = -1;
  long long bsize = -1;
 
  
    while (pos < stop) {
      long len;
    const long long id = ReadUInt(pReader, pos, len);
    assert(id >= 0);   
    assert((pos + len) <= stop);
//     const long long id = ReadID(pReader, pos, len);
//     if (id < 0 || (pos + len) > stop)
//       return E_FILE_FORMAT_INVALID;
  
      pos += len;   
  
  const long long size = ReadUInt(pReader, pos, len);
     assert(size >= 0);  
     assert((pos + len) <= stop);
 
     pos += len;  
 
  if (id == 0x21) {  
  if (bpos < 0) {  
         bpos = pos;
 
          bsize = size;
        }
      } else if (id == 0x1B) {   
      assert(size <= 8);
//       if (size > 8)
//         return E_FILE_FORMAT_INVALID;
  
        duration = UnserializeUInt(pReader, pos, size);
      assert(duration >= 0);   
// 
//       if (duration < 0)
//         return E_FILE_FORMAT_INVALID;
      } else if (id == 0x7B) {   
      assert(size <= 8);
//       if (size > 8 || size <= 0)
//         return E_FILE_FORMAT_INVALID;
        const long size_ = static_cast<long>(size);
  
        long long time;
 
  long status = UnserializeInt(pReader, pos, size_, time);
       assert(status == 0);
  if (status != 0)
  return -1;
 
  if (time <= 0)  
         prev = time;
  else  
         next = time;
 
      }
  
      pos += size;   
    assert(pos <= stop);
//     if (pos > stop)
//       return E_FILE_FORMAT_INVALID;
    }
//   if (bpos < 0)
//     return E_FILE_FORMAT_INVALID;
  
  assert(pos == stop);
  assert(bpos >= 0);
//   if (pos != stop)
//     return E_FILE_FORMAT_INVALID;
    assert(bsize >= 0);
  
    const long idx = m_entries_count;
 
  BlockEntry** const ppEntry = m_entries + idx;
  BlockEntry*& pEntry = *ppEntry;
 
   pEntry = new (std::nothrow)
  BlockGroup(this, idx, bpos, bsize, prev, next, duration, discard_padding);
 
  if (pEntry == NULL)
  return -1;  
 
  BlockGroup* const p = static_cast<BlockGroup*>(pEntry);
 
  const long status = p->Parse();
 
  if (status == 0) {  
  ++m_entries_count;
  return 0;
  }
 
  delete pEntry;
   pEntry = 0;
 
  return status;
 }