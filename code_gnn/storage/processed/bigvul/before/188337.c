 const BlockEntry* Cluster::GetEntry(
    const Track* pTrack,
    long long time_ns) const
{
    assert(pTrack);
    if (m_pSegment == NULL)   
        return pTrack->GetEOS();
//   if (m_pSegment == NULL)   
//     return pTrack->GetEOS();
  
  #if 0
  
  LoadBlockEntries();
 
  if ((m_entries == NULL) || (m_entries_count <= 0))
  return NULL;  
 
  const BlockEntry* pResult = pTrack->GetEOS();
 
  BlockEntry** i = m_entries;
     assert(i);
 
  BlockEntry** const j = i + m_entries_count;
 
  while (i != j)
  {
  const BlockEntry* const pEntry = *i++;
         assert(pEntry);
         assert(!pEntry->EOS());
 
  const Block* const pBlock = pEntry->GetBlock();
         assert(pBlock);
 
  if (pBlock->GetTrackNumber() != pTrack->GetNumber())
  continue;
 
  if (pTrack->VetEntry(pEntry))
  {
  if (time_ns < 0)  
  return pEntry;
 
  const long long ns = pBlock->GetTime(this);
 
  if (ns > time_ns)
  break;
 
             pResult = pEntry;
  }
  else if (time_ns >= 0)
  {
  const long long ns = pBlock->GetTime(this);
 
  if (ns > time_ns)
  break;
  }
  }
 
  return pResult;
 
  
  #else
  
    const BlockEntry* pResult = pTrack->GetEOS();
//   const BlockEntry* pResult = pTrack->GetEOS();
  
    long index = 0;
//   long index = 0;
  
    for (;;)
    {
        if (index >= m_entries_count)
        {
            long long pos;
            long len;
//   for (;;) {
//     if (index >= m_entries_count) {
//       long long pos;
//       long len;
  
            const long status = Parse(pos, len);
            assert(status >= 0);
//       const long status = Parse(pos, len);
//       assert(status >= 0);
  
            if (status > 0)   
                return pResult;
//       if (status > 0)   
//         return pResult;
  
            if (status < 0)   
                return 0;
//       if (status < 0)   
//         return 0;
  
            assert(m_entries);
            assert(index < m_entries_count);
        }
        const BlockEntry* const pEntry = m_entries[index];
        assert(pEntry);
        assert(!pEntry->EOS());
        const Block* const pBlock = pEntry->GetBlock();
        assert(pBlock);
        if (pBlock->GetTrackNumber() != pTrack->GetNumber())
        {
            ++index;
            continue;
        }
        if (pTrack->VetEntry(pEntry))
        {
            if (time_ns < 0)   
                return pEntry;
            const long long ns = pBlock->GetTime(this);
            if (ns > time_ns)
                return pResult;
            pResult = pEntry;   
        }
        else if (time_ns >= 0)
        {
            const long long ns = pBlock->GetTime(this);
            if (ns > time_ns)
                return pResult;
        }
        ++index;
//       assert(m_entries);
//       assert(index < m_entries_count);
      }
  
//     const BlockEntry* const pEntry = m_entries[index];
//     assert(pEntry);
//     assert(!pEntry->EOS());
// 
//     const Block* const pBlock = pEntry->GetBlock();
//     assert(pBlock);
// 
//     if (pBlock->GetTrackNumber() != pTrack->GetNumber()) {
//       ++index;
//       continue;
//     }
// 
//     if (pTrack->VetEntry(pEntry)) {
//       if (time_ns < 0)   
//         return pEntry;
// 
//       const long long ns = pBlock->GetTime(this);
// 
//       if (ns > time_ns)
//         return pResult;
// 
//       pResult = pEntry;   
//     } else if (time_ns >= 0) {
//       const long long ns = pBlock->GetTime(this);
// 
//       if (ns > time_ns)
//         return pResult;
//     }
// 
//     ++index;
//   }
// 
  #endif
  }