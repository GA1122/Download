IntRect Editor::FirstRectForRange(const EphemeralRange& range) const {
  DCHECK(!GetFrame().GetDocument()->NeedsLayoutTreeUpdate());
  DocumentLifecycle::DisallowTransitionScope disallow_transition(
      GetFrame().GetDocument()->Lifecycle());

  LayoutUnit extra_width_to_end_of_line;
  DCHECK(range.IsNotNull());

  IntRect start_caret_rect =
      RenderedPosition(
          CreateVisiblePosition(range.StartPosition()).DeepEquivalent(),
          TextAffinity::kDownstream)
          .AbsoluteRect(&extra_width_to_end_of_line);
  if (start_caret_rect.IsEmpty())
    return IntRect();

  IntRect end_caret_rect =
      RenderedPosition(
          CreateVisiblePosition(range.EndPosition()).DeepEquivalent(),
          TextAffinity::kUpstream)
          .AbsoluteRect();
  if (end_caret_rect.IsEmpty())
    return IntRect();

  if (start_caret_rect.Y() == end_caret_rect.Y()) {
    return IntRect(
        std::min(start_caret_rect.X(), end_caret_rect.X()),
        start_caret_rect.Y(), abs(end_caret_rect.X() - start_caret_rect.X()),
        std::max(start_caret_rect.Height(), end_caret_rect.Height()));
  }

  return IntRect(
      start_caret_rect.X(), start_caret_rect.Y(),
      (start_caret_rect.Width() + extra_width_to_end_of_line).ToInt(),
      start_caret_rect.Height());
}
