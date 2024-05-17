void RenderFrameImpl::InstallCreateHook(
    CreateRenderFrameImplFunction create_render_frame_impl) {
  CHECK(!g_create_render_frame_impl);
  g_create_render_frame_impl = create_render_frame_impl;
}
