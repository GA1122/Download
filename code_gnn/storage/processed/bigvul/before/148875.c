void RenderFrameHostManager::DidNavigateFrame(
    RenderFrameHostImpl* render_frame_host,
    bool was_caused_by_user_gesture) {
  CommitPendingIfNecessary(render_frame_host, was_caused_by_user_gesture);

  CommitPendingFramePolicy();
}
