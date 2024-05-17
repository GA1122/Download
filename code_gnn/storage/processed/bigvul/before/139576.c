static unsigned VerticalScrollDistance(LocalFrame& frame) {
  Element* focused_element = frame.GetDocument()->FocusedElement();
  if (!focused_element)
    return 0;
  LayoutObject* layout_object = focused_element->GetLayoutObject();
  if (!layout_object || !layout_object->IsBox())
    return 0;
  LayoutBox& layout_box = ToLayoutBox(*layout_object);
  const ComputedStyle* style = layout_box.Style();
  if (!style)
    return 0;
  if (!(style->OverflowY() == EOverflow::kScroll ||
        style->OverflowY() == EOverflow::kAuto ||
        HasEditableStyle(*focused_element)))
    return 0;
  int height = std::min<int>(layout_box.ClientHeight().ToInt(),
                             frame.View()->VisibleHeight());
  return static_cast<unsigned>(
      max(max<int>(height * ScrollableArea::MinFractionToStepWhenPaging(),
                   height - ScrollableArea::MaxOverlapBetweenPages()),
          1));
}
