PlainTextRange getSelectionOffsets(LocalFrame* frame) {
  EphemeralRange range = firstEphemeralRangeOf(
      frame->selection().computeVisibleSelectionInDOMTreeDeprecated());
  if (range.isNull())
    return PlainTextRange();
  ContainerNode* const editable = rootEditableElementOrTreeScopeRootNodeOf(
      frame->selection().computeVisibleSelectionInDOMTreeDeprecated());
  DCHECK(editable);
  return PlainTextRange::create(*editable, range);
}
