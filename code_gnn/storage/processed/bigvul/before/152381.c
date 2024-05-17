void RenderFrameImpl::InstallCreateHook(
    CreateRenderFrameImplFunction create_frame) {
  DCHECK(!g_create_render_frame_impl);
  g_create_render_frame_impl = create_frame;
}
