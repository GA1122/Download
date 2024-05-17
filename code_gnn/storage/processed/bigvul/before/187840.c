  const CuePoint* Cues::GetFirst() const {
  if (m_cue_points == NULL)
//   if (m_cue_points == NULL || m_count == 0)
      return NULL;
  
  if (m_count == 0)
    return NULL;
#if 0
    LoadCuePoint();   
    const size_t count = m_count + m_preload_count;
    if (count == 0)   
        return NULL;
#endif
    CuePoint* const* const pp = m_cue_points;
  assert(pp);
//   if (pp == NULL)
//     return NULL;
  
    CuePoint* const pCP = pp[0];
  assert(pCP);
  assert(pCP->GetTimeCode() >= 0);
//   if (pCP == NULL || pCP->GetTimeCode() < 0)
//     return NULL;
  
    return pCP;
  }