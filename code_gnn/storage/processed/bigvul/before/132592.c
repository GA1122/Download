bool WebKitTestController::PrepareForLayoutTest(
    const GURL& test_url,
    const base::FilePath& current_working_directory,
    bool enable_pixel_dumping,
    const std::string& expected_pixel_hash) {
  DCHECK(CalledOnValidThread());
  test_phase_ = DURING_TEST;
  current_working_directory_ = current_working_directory;
  enable_pixel_dumping_ = enable_pixel_dumping;
  expected_pixel_hash_ = expected_pixel_hash;
  test_url_ = test_url;
  printer_->reset();
  ShellBrowserContext* browser_context =
      ShellContentBrowserClient::Get()->browser_context();
  if (test_url.spec().find("compositing/") != std::string::npos)
    is_compositing_test_ = true;
  initial_size_ = Shell::GetShellDefaultSize();
  if (test_url.spec().find("W3C-SVG-1.1") != std::string::npos)
    initial_size_ = gfx::Size(kTestSVGWindowWidthDip, kTestSVGWindowHeightDip);
  if (!main_window_) {
    main_window_ = content::Shell::CreateNewWindow(
        browser_context,
        GURL(),
        NULL,
        initial_size_);
    WebContentsObserver::Observe(main_window_->web_contents());
    send_configuration_to_next_host_ = true;
    current_pid_ = base::kNullProcessId;
    main_window_->LoadURL(test_url);
  } else {
#if defined(OS_MACOSX)
    main_window_->SizeTo(initial_size_);
#endif
    main_window_->web_contents()->GetRenderViewHost()->GetView()
        ->SetSize(initial_size_);
    main_window_->web_contents()->GetRenderViewHost()->WasResized();
    RenderViewHost* render_view_host =
        main_window_->web_contents()->GetRenderViewHost();
    WebPreferences prefs = render_view_host->GetWebkitPreferences();
    OverrideWebkitPrefs(&prefs);
    render_view_host->UpdateWebkitPreferences(prefs);
    SendTestConfiguration();

    NavigationController::LoadURLParams params(test_url);
    params.transition_type = ui::PageTransitionFromInt(
        ui::PAGE_TRANSITION_TYPED | ui::PAGE_TRANSITION_FROM_ADDRESS_BAR);
    params.should_clear_history_list = true;
    main_window_->web_contents()->GetController().LoadURLWithParams(params);
    main_window_->web_contents()->Focus();
  }
  main_window_->web_contents()->GetRenderViewHost()->SetActive(true);
  main_window_->web_contents()->GetRenderViewHost()->Focus();
  return true;
}
