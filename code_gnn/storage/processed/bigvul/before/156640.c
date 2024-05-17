void CheckFrameDepth(unsigned int expected_depth, FrameTreeNode* node) {
  EXPECT_EQ(expected_depth, node->depth());
  RenderProcessHost::Priority priority =
      node->current_frame_host()->GetRenderWidgetHost()->GetPriority();
  EXPECT_EQ(expected_depth, priority.frame_depth);
  EXPECT_EQ(expected_depth,
            node->current_frame_host()->GetProcess()->GetFrameDepth());
}
