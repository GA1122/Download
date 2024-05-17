void Editor::revealSelectionAfterEditingOperation(
    const ScrollAlignment& alignment,
    RevealExtentOption revealExtentOption) {
  if (m_preventRevealSelection)
    return;
  if (!frame().selection().isAvailable())
    return;
  frame().selection().revealSelection(alignment, revealExtentOption);
}
