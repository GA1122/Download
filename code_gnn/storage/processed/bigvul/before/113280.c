void PanelBrowserView::Close() {
  GetWidget()->RemoveObserver(this);
  closed_ = true;

  if (bounds_animator_.get())
    bounds_animator_.reset();

  ::BrowserView::Close();
}
