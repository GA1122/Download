gfx::ImageSkia BrowserView::GetWindowIcon() {
  if (browser_->is_devtools())
    return gfx::ImageSkia();

  extensions::HostedAppBrowserController* app_controller =
      browser()->hosted_app_controller();
  if (app_controller)
    return app_controller->GetWindowIcon();

#if defined(OS_CHROMEOS)
  if (browser_->is_type_tabbed()) {
    ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
    return rb.GetImageNamed(IDR_PRODUCT_LOGO_32).AsImageSkia();
  }

  if (!browser_->is_app() && browser_->is_trusted_source() &&
      GetNativeWindow()) {
    auto* image = GetNativeWindow()->GetProperty(aura::client::kWindowIconKey);
    if (image)
      return *image;
  }
#endif

  if (browser_->is_app() || browser_->is_type_popup())
    return browser_->GetCurrentPageIcon().AsImageSkia();

  return gfx::ImageSkia();
}
