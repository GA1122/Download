void DevToolsWindow::InspectedContentsClosing() {
  if (!close_on_detach_)
    return;
  intercepted_page_beforeunload_ = false;
  life_stage_ = kClosing;
  main_web_contents_->ClosePage();
}
