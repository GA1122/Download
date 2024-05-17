void FrameSelection::MoveCaretSelection(const IntPoint& point) {
  DCHECK(!GetDocument().NeedsLayoutTreeUpdate());

  Element* const editable =
      ComputeVisibleSelectionInDOMTree().RootEditableElement();
  if (!editable)
    return;

  const VisiblePosition position =
      VisiblePositionForContentsPoint(point, GetFrame());
  SelectionInDOMTree::Builder builder;
  if (position.IsNotNull())
    builder.Collapse(position.ToPositionWithAffinity());
  SetSelection(builder.Build(), SetSelectionOptions::Builder()
                                    .SetShouldCloseTyping(true)
                                    .SetShouldClearTypingStyle(true)
                                    .SetSetSelectionBy(SetSelectionBy::kUser)
                                    .SetShouldShowHandle(true)
                                    .SetIsDirectional(IsDirectional())
                                    .Build());
}
