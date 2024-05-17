void Editor::addToKillRing(const EphemeralRange& range) {
  if (m_shouldStartNewKillRingSequence)
    killRing().startNewSequence();

  DCHECK(!frame().document()->needsLayoutTreeUpdate());
  String text = plainText(range);
  killRing().append(text);
  m_shouldStartNewKillRingSequence = false;
}
