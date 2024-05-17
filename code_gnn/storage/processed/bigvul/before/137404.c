void RenderViewTest::Resize(gfx::Size new_size,
                            bool is_fullscreen_granted) {
  ResizeParams params;
  params.screen_info = ScreenInfo();
  params.new_size = new_size;
  params.physical_backing_size = new_size;
  params.top_controls_height = 0.f;
  params.browser_controls_shrink_blink_size = false;
  params.is_fullscreen_granted = is_fullscreen_granted;
  params.display_mode = blink::kWebDisplayModeBrowser;
  std::unique_ptr<IPC::Message> resize_message(new ViewMsg_Resize(0, params));
  OnMessageReceived(*resize_message);
}
