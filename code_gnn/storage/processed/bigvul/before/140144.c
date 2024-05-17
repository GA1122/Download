void HTMLMediaElement::viewportFillDebouncerTimerFired(TimerBase*) {
  m_mostlyFillingViewport = true;
  if (m_webMediaPlayer)
    m_webMediaPlayer->becameDominantVisibleContent(m_mostlyFillingViewport);
}
