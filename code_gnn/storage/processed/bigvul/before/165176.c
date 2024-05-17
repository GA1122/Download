bool DragController::ConcludeEditDrag(DragData* drag_data) {
  DCHECK(drag_data);

  HTMLInputElement* file_input = file_input_element_under_mouse_;
  if (file_input_element_under_mouse_) {
    file_input_element_under_mouse_->SetCanReceiveDroppedFiles(false);
    file_input_element_under_mouse_ = nullptr;
  }

  if (!document_under_mouse_)
    return false;

  LayoutPoint point = document_under_mouse_->View()->RootFrameToContents(
      LayoutPoint(drag_data->ClientPosition()));
  Element* element = ElementUnderMouse(document_under_mouse_.Get(), point);
  if (!element)
    return false;
  LocalFrame* inner_frame = element->ownerDocument()->GetFrame();
  DCHECK(inner_frame);

  if (page_->GetDragCaret().HasCaret() &&
      DispatchTextInputEventFor(inner_frame, drag_data) !=
          DispatchEventResult::kNotCanceled)
    return true;

  if (drag_data->ContainsFiles() && file_input) {
    if (file_input->GetLayoutObject())
      DCHECK_EQ(file_input, element);
    if (file_input->IsDisabledFormControl())
      return false;

    return file_input->ReceiveDroppedFiles(drag_data);
  }

  if (!page_->GetDragController().CanProcessDrag(
          drag_data, inner_frame->LocalFrameRoot())) {
    page_->GetDragCaret().Clear();
    return false;
  }

  if (page_->GetDragCaret().HasCaret()) {
    page_->GetDragCaret()
        .CaretPosition()
        .GetPosition()
        .GetDocument()
        ->UpdateStyleAndLayoutIgnorePendingStylesheets();
  }

  const PositionWithAffinity& caret_position =
      page_->GetDragCaret().CaretPosition();
  if (!caret_position.IsConnected()) {
    page_->GetDragCaret().Clear();
    return false;
  }
  VisibleSelection drag_caret = CreateVisibleSelection(
      SelectionInDOMTree::Builder().Collapse(caret_position).Build());
  page_->GetDragCaret().Clear();
  if (!inner_frame->Selection().IsAvailable()) {
    return false;
  }
  Range* range = CreateRange(drag_caret.ToNormalizedEphemeralRange());
  Element* root_editable_element =
      inner_frame->Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .RootEditableElement();

  if (!range)
    return false;
  ResourceFetcher* fetcher = range->OwnerDocument().Fetcher();
  ResourceCacheValidationSuppressor validation_suppressor(fetcher);

  inner_frame->GetEditor().RegisterCommandGroup(
      DragAndDropCommand::Create(*inner_frame->GetDocument()));

  if (DragIsMove(inner_frame->Selection(), drag_data) ||
      IsRichlyEditablePosition(drag_caret.Base())) {
    DragSourceType drag_source_type = DragSourceType::kHTMLSource;
    DocumentFragment* fragment = DocumentFragmentFromDragData(
        drag_data, inner_frame, range, true, drag_source_type);
    if (!fragment)
      return false;

    if (DragIsMove(inner_frame->Selection(), drag_data)) {
      const DeleteMode delete_mode =
          inner_frame->GetEditor().SmartInsertDeleteEnabled()
              ? DeleteMode::kSmart
              : DeleteMode::kSimple;
      const InsertMode insert_mode =
          (delete_mode == DeleteMode::kSmart &&
           inner_frame->Selection().Granularity() == TextGranularity::kWord &&
           drag_data->CanSmartReplace())
              ? InsertMode::kSmart
              : InsertMode::kSimple;

      if (!inner_frame->GetEditor().DeleteSelectionAfterDraggingWithEvents(
              inner_frame->GetEditor().FindEventTargetFrom(
                  inner_frame->Selection()
                      .ComputeVisibleSelectionInDOMTreeDeprecated()),
              delete_mode, drag_caret.Base()))
        return false;

      inner_frame->Selection().SetSelectionAndEndTyping(
          SelectionInDOMTree::Builder()
              .SetBaseAndExtent(EphemeralRange(range))
              .Build());
      if (inner_frame->Selection().IsAvailable()) {
        DCHECK(document_under_mouse_);
        if (!inner_frame->GetEditor().ReplaceSelectionAfterDraggingWithEvents(
                element, drag_data, fragment, range, insert_mode,
                drag_source_type))
          return false;
      }
    } else {
      if (SetSelectionToDragCaret(inner_frame, drag_caret, range, point)) {
        DCHECK(document_under_mouse_);
        if (!inner_frame->GetEditor().ReplaceSelectionAfterDraggingWithEvents(
                element, drag_data, fragment, range,
                drag_data->CanSmartReplace() ? InsertMode::kSmart
                                             : InsertMode::kSimple,
                drag_source_type))
          return false;
      }
    }
  } else {
    String text = drag_data->AsPlainText();
    if (text.IsEmpty())
      return false;

    if (SetSelectionToDragCaret(inner_frame, drag_caret, range, point)) {
      DCHECK(document_under_mouse_);
      if (!inner_frame->GetEditor().ReplaceSelectionAfterDraggingWithEvents(
              element, drag_data,
              CreateFragmentFromText(EphemeralRange(range), text), range,
              InsertMode::kSimple, DragSourceType::kPlainTextSource))
        return false;
    }
  }

  if (root_editable_element) {
    if (LocalFrame* frame = root_editable_element->GetDocument().GetFrame()) {
      frame->GetEventHandler().UpdateDragStateAfterEditDragIfNeeded(
          root_editable_element);
    }
  }

  return true;
}
