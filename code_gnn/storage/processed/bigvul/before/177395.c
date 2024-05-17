const BlockEntry* Cluster::GetEntry(const Track* pTrack,
 long long time_ns) const {
  assert(pTrack);

 if (m_pSegment == NULL)  
 return pTrack->GetEOS();

 const BlockEntry* pResult = pTrack->GetEOS();

 long index = 0;

 for (;;) {
 if (index >= m_entries_count) {
 long long pos;
 long len;

 const long status = Parse(pos, len);
      assert(status >= 0);

 if (status > 0)  
 return pResult;

 if (status < 0)  
 return 0;

      assert(m_entries);
      assert(index < m_entries_count);
 }

 const BlockEntry* const pEntry = m_entries[index];
    assert(pEntry);
    assert(!pEntry->EOS());

 const Block* const pBlock = pEntry->GetBlock();
    assert(pBlock);

 if (pBlock->GetTrackNumber() != pTrack->GetNumber()) {
 ++index;
 continue;
 }

 if (pTrack->VetEntry(pEntry)) {
 if (time_ns < 0)  
 return pEntry;

 const long long ns = pBlock->GetTime(this);

 if (ns > time_ns)
 return pResult;

      pResult = pEntry;  
 } else if (time_ns >= 0) {
 const long long ns = pBlock->GetTime(this);

 if (ns > time_ns)
 return pResult;
 }

 ++index;
 }
}
