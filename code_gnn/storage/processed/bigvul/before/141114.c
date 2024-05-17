void Document::HoveredElementDetached(Element& element) {
  if (!hover_element_)
    return;
  if (element != hover_element_)
    return;
  hover_element_ =
      SkipDisplayNoneAncestorsOrReturnNullIfFlatTreeIsDirty(element);

  if (!GetPage()->IsCursorVisible())
    return;

  if (GetFrame())
    GetFrame()->GetEventHandler().ScheduleHoverStateUpdate();
}
