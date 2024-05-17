void WebUILoginView::ReturnFocus(bool reverse) {
  webui_login_->web_contents()->FocusThroughTabTraversal(reverse);
  GetWidget()->Activate();
}
