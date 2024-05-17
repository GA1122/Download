bool HTMLMediaElement::shouldAutoplay() {
  if (document().isSandboxed(SandboxAutomaticFeatures))
    return false;
  return m_autoplaying && m_paused && autoplay();
}
