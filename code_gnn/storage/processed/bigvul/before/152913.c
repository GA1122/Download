void DevToolsWindow::InspectedContentsClosing() {
  intercepted_page_beforeunload_ = false;
  life_stage_ = kClosing;
  main_web_contents_->ClosePage();
}
