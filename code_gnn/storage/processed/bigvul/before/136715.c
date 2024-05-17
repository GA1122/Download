bool FrameSelection::Modify(SelectionModifyAlteration alter,
                            SelectionModifyDirection direction,
                            TextGranularity granularity,
                            SetSelectionBy set_selection_by) {
  SelectionModifier selection_modifier(*GetFrame(), GetSelectionInDOMTree(),
                                       x_pos_for_vertical_arrow_navigation_);
  selection_modifier.SetSelectionIsDirectional(IsDirectional());
  const bool modified =
      selection_modifier.Modify(alter, direction, granularity);
  if (set_selection_by == SetSelectionBy::kUser &&
      selection_modifier.Selection().IsRange() &&
      ComputeVisibleSelectionInDOMTree().IsCaret() &&
      DispatchSelectStart(ComputeVisibleSelectionInDOMTree()) !=
          DispatchEventResult::kNotCanceled) {
    return false;
  }
  if (!modified) {
    if (set_selection_by == SetSelectionBy::kSystem)
      return false;
    if (IsSpatialNavigationEnabled(frame_))
      return false;
    return true;
  }

  const bool selection_is_directional =
      alter == SelectionModifyAlteration::kExtend ||
      ShouldAlwaysUseDirectionalSelection(frame_);

  SetSelection(selection_modifier.Selection().AsSelection(),
               SetSelectionOptions::Builder()
                   .SetShouldCloseTyping(true)
                   .SetShouldClearTypingStyle(true)
                   .SetSetSelectionBy(set_selection_by)
                   .SetIsDirectional(selection_is_directional)
                   .Build());

  if (granularity == TextGranularity::kLine ||
      granularity == TextGranularity::kParagraph)
    x_pos_for_vertical_arrow_navigation_ =
        selection_modifier.XPosForVerticalArrowNavigation();

  if (set_selection_by == SetSelectionBy::kUser)
    granularity_ = TextGranularity::kCharacter;

  ScheduleVisualUpdateForPaintInvalidationIfNeeded();

  return true;
}
