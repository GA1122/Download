  bool Cues::Find(long long time_ns, const Track* pTrack, const CuePoint*& pCP,
                  const CuePoint::TrackPosition*& pTP) const {
  assert(time_ns >= 0);
  assert(pTrack);
#if 0
    LoadCuePoint();   
    assert(m_cue_points);
    assert(m_count > 0);
    CuePoint** const ii = m_cue_points;
    CuePoint** i = ii;
    CuePoint** const jj = ii + m_count + m_preload_count;
    CuePoint** j = jj;
    pCP = *i;
    assert(pCP);
    if (time_ns <= pCP->GetTime(m_pSegment))
    {
        pTP = pCP->Find(pTrack);
        return (pTP != NULL);
    }
    IMkvReader* const pReader = m_pSegment->m_pReader;
    while (i < j)
    {
        CuePoint** const k = i + (j - i) / 2;
        assert(k < jj);
        CuePoint* const pCP = *k;
        assert(pCP);
        pCP->Load(pReader);
        const long long t = pCP->GetTime(m_pSegment);
        if (t <= time_ns)
            i = k + 1;
        else
            j = k;
        assert(i <= j);
    }
    assert(i == j);
    assert(i <= jj);
    assert(i > ii);
    pCP = *--i;
    assert(pCP);
    assert(pCP->GetTime(m_pSegment) <= time_ns);
#else
  if (m_cue_points == NULL)
    return false;
  if (m_count == 0)
//   if (time_ns < 0 || pTrack == NULL || m_cue_points == NULL || m_count == 0)
      return false;
  
    CuePoint** const ii = m_cue_points;
  CuePoint** i = ii;
 
  CuePoint** const jj = ii + m_count;
 
    CuePoint** j = jj;
  
    pCP = *i;
  assert(pCP);
//   if (pCP == NULL)
//     return false;
  
    if (time_ns <= pCP->GetTime(m_pSegment)) {
      pTP = pCP->Find(pTrack);
  return (pTP != NULL);
  }
 
  while (i < j) {
 
  
      CuePoint** const k = i + (j - i) / 2;
    assert(k < jj);
//     if (k >= jj)
//       return false;
  
      CuePoint* const pCP = *k;
    assert(pCP);
//     if (pCP == NULL)
//       return false;
  
      const long long t = pCP->GetTime(m_pSegment);
  
  if (t <= time_ns)
       i = k + 1;
 
      else
        j = k;
  
    assert(i <= j);
//     if (i > j)
//       return false;
    }
  
  assert(i == j);
  assert(i <= jj);
  assert(i > ii);
//   if (i != j || i > jj || i <= ii)
//     return false;
  
    pCP = *--i;
  assert(pCP);
  assert(pCP->GetTime(m_pSegment) <= time_ns);
#endif
// 
//   if (pCP == NULL || pCP->GetTime(m_pSegment) > time_ns)
//     return false;
  
 
   pTP = pCP->Find(pTrack);
 
    return (pTP != NULL);
  }