bool TestRenderViewHost::CreateTestRenderView(
    const base::string16& frame_name,
    int opener_frame_route_id,
    int proxy_route_id,
    bool window_was_created_with_opener) {
  FrameReplicationState replicated_state;
  replicated_state.name = base::UTF16ToUTF8(frame_name);
  return CreateRenderView(opener_frame_route_id, proxy_route_id,
                          replicated_state, window_was_created_with_opener);
}
