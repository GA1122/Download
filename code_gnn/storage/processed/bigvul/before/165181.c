DispatchEventResult DragController::DispatchTextInputEventFor(
    LocalFrame* inner_frame,
    DragData* drag_data) {
  DCHECK(!inner_frame->GetDocument()->NeedsLayoutTreeUpdate());
  DCHECK(page_->GetDragCaret().HasCaret());
  String text = page_->GetDragCaret().IsContentRichlyEditable()
                    ? ""
                    : drag_data->AsPlainText();
  const PositionWithAffinity& caret_position =
      page_->GetDragCaret().CaretPosition();
  DCHECK(caret_position.IsConnected()) << caret_position;
  Element* target =
      inner_frame->GetEditor().FindEventTargetFrom(CreateVisibleSelection(
          SelectionInDOMTree::Builder().Collapse(caret_position).Build()));
  return target->DispatchEvent(
      TextEvent::CreateForDrop(inner_frame->DomWindow(), text));
}
