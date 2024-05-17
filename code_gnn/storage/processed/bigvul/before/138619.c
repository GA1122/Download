  TestRenderFrameHost* GetSpeculativeRenderFrameHost(FrameTreeNode* node) {
    return static_cast<TestRenderFrameHost*>(
        node->render_manager()->speculative_render_frame_host_.get());
  }
