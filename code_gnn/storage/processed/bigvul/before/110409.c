void WebUILoginView::LoadURL(const GURL & url) {
  webui_login_->LoadInitialURL(url);
  webui_login_->RequestFocus();

  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (BaseLoginDisplayHost::default_host() &&
      command_line->HasSwitch(switches::kEnableNewOobe)) {
    SkBitmap background;
    background.setConfig(SkBitmap::kARGB_8888_Config, 1, 1);
    background.allocPixels();
    background.eraseARGB(0x00, 0x00, 0x00, 0x00);
    content::RenderViewHost* host =
        tab_contents_->web_contents()->GetRenderViewHost();
    host->GetView()->SetBackground(background);
  }
}
