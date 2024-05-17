void WebPluginDelegatePepper::SelectedFindResultChanged(int index) {
  render_view_->reportFindInPageSelection(
        find_identifier_, index + 1, WebKit::WebRect());
}
