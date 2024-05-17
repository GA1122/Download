void SelectionController::SendContextMenuEvent(
    const MouseEventWithHitTestResults& mev,
    const LayoutPoint& position) {
  if (!Selection().IsAvailable())
    return;
  if (Selection().Contains(position) || mev.GetScrollbar() ||
      !(Selection()
            .ComputeVisibleSelectionInDOMTreeDeprecated()
            .IsContentEditable() ||
        (mev.InnerNode() && mev.InnerNode()->IsTextNode())))
    return;

  AutoReset<bool> mouse_down_may_start_select_change(
      &mouse_down_may_start_select_, true);

  if (mev.Event().menu_source_type != kMenuSourceTouchHandle &&
      HitTestResultIsMisspelled(mev.GetHitTestResult()))
    return SelectClosestMisspellingFromMouseEvent(mev);

  if (!frame_->GetEditor().Behavior().ShouldSelectOnContextualMenuClick())
    return;

  SelectClosestWordOrLinkFromMouseEvent(mev);
}
