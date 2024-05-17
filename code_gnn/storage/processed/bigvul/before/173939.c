long Cluster::GetFirst(const BlockEntry*& pFirst) const {
 if (m_entries_count <= 0) {
 long long pos;
 long len;

 const long status = Parse(pos, len);

 if (status < 0) {  
      pFirst = NULL;
 return status;
 }

 if (m_entries_count <= 0) {  
      pFirst = NULL;
 return 0;
 }
 }

  assert(m_entries);

  pFirst = m_entries[0];
  assert(pFirst);

 return 0;  
}
