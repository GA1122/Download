void RenderViewImpl::OnUpdateScreenInfo(const ScreenInfo& screen_info) {
  if (!main_render_frame_)
    GetWidget()->set_screen_info(screen_info);
}
