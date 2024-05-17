void Editor::setMarkedTextMatchesAreHighlighted(bool flag) {
  if (flag == m_areMarkedTextMatchesHighlighted)
    return;

  m_areMarkedTextMatchesHighlighted = flag;
  frame().document()->markers().repaintMarkers(DocumentMarker::TextMatch);
}
