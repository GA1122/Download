CueTimeline& HTMLMediaElement::cueTimeline() {
  if (!m_cueTimeline)
    m_cueTimeline = new CueTimeline(*this);
  return *m_cueTimeline;
}
