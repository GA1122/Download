Editor::RevealSelectionScope::~RevealSelectionScope() {
  DCHECK(m_editor->m_preventRevealSelection);
  --m_editor->m_preventRevealSelection;
  if (!m_editor->m_preventRevealSelection) {
    m_editor->frame().selection().revealSelection(
        ScrollAlignment::alignToEdgeIfNeeded, RevealExtent);
  }
}
