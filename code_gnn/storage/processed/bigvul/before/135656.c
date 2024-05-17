void FrameSelection::NotifyCompositorForSelectionChange() {
  if (!RuntimeEnabledFeatures::CompositedSelectionUpdateEnabled())
    return;

  ScheduleVisualUpdate();
}
