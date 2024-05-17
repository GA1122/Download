ShellWindowViews::ShellWindowViews(Profile* profile,
                                   const extensions::Extension* extension,
                                   const GURL& url,
                                   const ShellWindow::CreateParams& win_params)
    : ShellWindow(profile, extension, url),
      web_view_(NULL),
      is_fullscreen_(false),
      use_custom_frame_(
          win_params.frame == ShellWindow::CreateParams::FRAME_NONE) {
  window_ = new views::Widget;
  views::Widget::InitParams params(views::Widget::InitParams::TYPE_WINDOW);
  params.delegate = this;
  params.remove_standard_frame = true;
  minimum_size_ = win_params.minimum_size;
  maximum_size_ = win_params.maximum_size;
  window_->Init(params);
  gfx::Rect window_bounds =
      window_->non_client_view()->GetWindowBoundsForClientBounds(
          win_params.bounds);
  window_->SetBounds(window_bounds);
#if defined(OS_WIN) && !defined(USE_AURA)
  std::string app_name = web_app::GenerateApplicationNameFromExtensionId(
      extension->id());
  ui::win::SetAppIdForWindow(
      ShellIntegration::GetAppModelIdForProfile(UTF8ToWide(app_name),
                                                profile->GetPath()),
      GetWidget()->GetTopLevelWidget()->GetNativeWindow());
#endif
  OnViewWasResized();

  window_->Show();
}
