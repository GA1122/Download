void HTMLInputElement::UpdateFocusAppearanceWithOptions(
    SelectionBehaviorOnFocus selection_behavior,
    const FocusOptions& options) {
  if (IsTextField()) {
    switch (selection_behavior) {
      case SelectionBehaviorOnFocus::kReset:
        select();
        break;
      case SelectionBehaviorOnFocus::kRestore:
        RestoreCachedSelection();
        break;
      case SelectionBehaviorOnFocus::kNone:
        return;
    }
    GetDocument().EnsurePaintLocationDataValidForNode(this);
    if (!options.preventScroll()) {
      if (GetLayoutObject()) {
        GetLayoutObject()->ScrollRectToVisible(BoundingBoxForScrollIntoView(),
                                               WebScrollIntoViewParams());
      }
      if (GetDocument().GetFrame())
        GetDocument().GetFrame()->Selection().RevealSelection();
    }
  } else {
    TextControlElement::UpdateFocusAppearanceWithOptions(selection_behavior,
                                                         options);
  }
}
