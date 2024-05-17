IntRect Editor::firstRectForRange(const EphemeralRange& range) const {
  DCHECK(!frame().document()->needsLayoutTreeUpdate());
  DocumentLifecycle::DisallowTransitionScope disallowTransition(
      frame().document()->lifecycle());

  LayoutUnit extraWidthToEndOfLine;
  DCHECK(range.isNotNull());

  IntRect startCaretRect =
      RenderedPosition(
          createVisiblePosition(range.startPosition()).deepEquivalent(),
          TextAffinity::Downstream)
          .absoluteRect(&extraWidthToEndOfLine);
  if (startCaretRect.isEmpty())
    return IntRect();

  IntRect endCaretRect =
      RenderedPosition(
          createVisiblePosition(range.endPosition()).deepEquivalent(),
          TextAffinity::Upstream)
          .absoluteRect();
  if (endCaretRect.isEmpty())
    return IntRect();

  if (startCaretRect.y() == endCaretRect.y()) {
    return IntRect(std::min(startCaretRect.x(), endCaretRect.x()),
                   startCaretRect.y(),
                   abs(endCaretRect.x() - startCaretRect.x()),
                   std::max(startCaretRect.height(), endCaretRect.height()));
  }

  return IntRect(startCaretRect.x(), startCaretRect.y(),
                 (startCaretRect.width() + extraWidthToEndOfLine).toInt(),
                 startCaretRect.height());
}
