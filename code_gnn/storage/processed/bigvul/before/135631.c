static DispatchEventResult DispatchSelectStart(
    const VisibleSelection& selection) {
  Node* select_start_target = selection.Extent().ComputeContainerNode();
  if (!select_start_target)
    return DispatchEventResult::kNotCanceled;

  return select_start_target->DispatchEvent(
      Event::CreateCancelableBubble(EventTypeNames::selectstart));
}
