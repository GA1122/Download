gfx::ImageSkia BrowserView::GetWindowAppIcon() {
  extensions::HostedAppBrowserController* app_controller =
      browser()->hosted_app_controller();
  return app_controller ? app_controller->GetWindowAppIcon() : GetWindowIcon();
}
