void DevToolsWindow::CloseWindow() {
  DCHECK(is_docked_);
  life_stage_ = kClosing;
  main_web_contents_->DispatchBeforeUnload();
}
