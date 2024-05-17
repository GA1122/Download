void Editor::SetMarkedTextMatchesAreHighlighted(bool flag) {
  if (flag == are_marked_text_matches_highlighted_)
    return;

  are_marked_text_matches_highlighted_ = flag;
  GetFrame().GetDocument()->Markers().RepaintMarkers(
      DocumentMarker::kTextMatch);
}
