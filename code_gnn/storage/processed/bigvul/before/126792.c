gfx::ImageSkia BrowserView::GetWindowIcon() {
  if (browser_->is_app())
    return browser_->GetCurrentPageIcon().AsImageSkia();
  return gfx::ImageSkia();
}
