bool CheckIntersectsViewport(bool expected, FrameTreeNode* node) {
  RenderProcessHost::Priority priority =
      node->current_frame_host()->GetRenderWidgetHost()->GetPriority();
  return priority.intersects_viewport == expected &&
         node->current_frame_host()->GetProcess()->GetIntersectsViewport() ==
             expected;
}
