void Document::HoveredElementDetached(Element& element) {
  if (!hover_element_)
    return;
  if (element != hover_element_)
    return;

  if (IsSlotAssignmentOrLegacyDistributionDirty()) {
    hover_element_ = nullptr;
  } else {
    hover_element_ = SkipDisplayNoneAncestors(&element);
  }

  if (!GetPage()->IsCursorVisible())
    return;

  if (GetFrame())
    GetFrame()->GetEventHandler().ScheduleHoverStateUpdate();
}
