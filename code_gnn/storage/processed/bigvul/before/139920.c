DEFINE_TRACE_WRAPPERS(HTMLMediaElement) {
  visitor->traceWrappers(m_videoTracks);
  visitor->traceWrappers(m_audioTracks);
  visitor->traceWrappers(m_textTracks);
  HTMLElement::traceWrappers(visitor);
}
