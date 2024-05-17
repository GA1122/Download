 const CuePoint::TrackPosition* CuePoint::Find(const Track* pTrack) const {
  assert(pTrack);

 const long long n = pTrack->GetNumber();

 const TrackPosition* i = m_track_positions;
 const TrackPosition* const j = i + m_track_positions_count;

 while (i != j) {
 const TrackPosition& p = *i++;

 if (p.m_track == n)
 return &p;
 }

 return NULL;  
}