bool BrowserRootView::OnMouseWheel(const ui::MouseWheelEvent& event) {
  if (browser_defaults::kScrollEventChangesTab) {
    views::View* hit_view = GetEventHandlerForPoint(event.location());
    views::NonClientView* non_client = GetWidget()->non_client_view();
    if (tabstrip()->Contains(hit_view) ||
        hit_view == non_client->frame_view()) {
      int scroll_offset = abs(event.y_offset()) > abs(event.x_offset()) ?
          event.y_offset() : -event.x_offset();
      Browser* browser = browser_view_->browser();
      TabStripModel* model = browser->tab_strip_model();
      if (scroll_offset < 0 && model->active_index() + 1 < model->count()) {
        chrome::SelectNextTab(browser);
        return true;
      }

      if (scroll_offset > 0 && model->active_index() > 0) {
        chrome::SelectPreviousTab(browser);
        return true;
      }
    }
  }
  return RootView::OnMouseWheel(event);
}
