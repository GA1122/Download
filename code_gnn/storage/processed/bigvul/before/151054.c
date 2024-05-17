void DevToolsWindow::CloseContents(WebContents* source) {
  CHECK(is_docked_);
  life_stage_ = kClosing;
  UpdateBrowserWindow();
  delete main_web_contents_;
}
