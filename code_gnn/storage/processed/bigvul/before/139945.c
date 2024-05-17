void HTMLMediaElement::checkViewportIntersectionTimerFired(TimerBase*) {
  bool shouldReportRootBounds = true;
  IntersectionGeometry geometry(nullptr, *this, Vector<Length>(),
                                shouldReportRootBounds);
  geometry.computeGeometry();
  IntRect intersectRect = geometry.intersectionIntRect();
  if (m_currentIntersectRect == intersectRect)
    return;

  m_currentIntersectRect = intersectRect;
  m_viewportFillDebouncerTimer.stop();
  bool isMostlyFillingViewport =
      (m_currentIntersectRect.size().area() >
       kMostlyFillViewportThreshold * geometry.rootIntRect().size().area());
  if (m_mostlyFillingViewport == isMostlyFillingViewport)
    return;

  if (!isMostlyFillingViewport) {
    m_mostlyFillingViewport = isMostlyFillingViewport;
    if (m_webMediaPlayer)
      m_webMediaPlayer->becameDominantVisibleContent(m_mostlyFillingViewport);
    return;
  }

  m_viewportFillDebouncerTimer.startOneShot(
      kMostlyFillViewportBecomeStableSeconds, BLINK_FROM_HERE);
}
