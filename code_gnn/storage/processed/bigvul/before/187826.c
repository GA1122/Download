 Cluster* Cluster::Create(Segment* pSegment, long idx, long long off)
{
  assert(pSegment);
  assert(off >= 0);
// Cluster* Cluster::Create(Segment* pSegment, long idx, long long off) {
//   if (!pSegment || off < 0)
//     return NULL;
  
    const long long element_start = pSegment->m_start + off;
  
  Cluster* const pCluster = new Cluster(pSegment, idx, element_start);
  assert(pCluster);
//   Cluster* const pCluster =
//       new (std::nothrow) Cluster(pSegment, idx, element_start);
  
    return pCluster;
  }