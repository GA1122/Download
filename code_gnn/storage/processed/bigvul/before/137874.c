void MediaControlCastButtonElement::recordMetrics(CastOverlayMetrics metric) {
  DCHECK(m_isOverlayButton);
  DEFINE_STATIC_LOCAL(
      EnumerationHistogram, overlayHistogram,
      ("Cast.Sender.Overlay", static_cast<int>(CastOverlayMetrics::Count)));
  overlayHistogram.count(static_cast<int>(metric));
}
