void FrameSelection::SetFocusedNodeIfNeeded() {
  if (ComputeVisibleSelectionInDOMTreeDeprecated().IsNone() ||
      !FrameIsFocused())
    return;

  if (Element* target =
          ComputeVisibleSelectionInDOMTreeDeprecated().RootEditableElement()) {
    GetDocument().UpdateStyleAndLayoutTreeIgnorePendingStylesheets();
    while (target) {
      if (target->IsMouseFocusable() && !IsFrameElement(target)) {
        frame_->GetPage()->GetFocusController().SetFocusedElement(target,
                                                                  frame_);
        return;
      }
      target = target->ParentOrShadowHostElement();
    }
    GetDocument().ClearFocusedElement();
  }
}
