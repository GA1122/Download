Cluster::Cluster(Segment* pSegment, long idx, long long element_start
  )
 : m_pSegment(pSegment),
      m_element_start(element_start),
      m_index(idx),
      m_pos(element_start),
      m_element_size(-1  ),
      m_timecode(-1),
      m_entries(NULL),
      m_entries_size(0),
      m_entries_count(-1)  
{}
