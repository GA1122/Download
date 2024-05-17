void BrowserView::InitViews() {
  GetWidget()->AddObserver(this);

  GetWidget()->SetNativeWindowProperty(kBrowserViewKey, this);

  GetWidget()->SetNativeWindowProperty(Profile::kProfileKey,
                                       browser_->profile());

#if defined(OS_WIN) || (defined(OS_LINUX) && !defined(OS_CHROMEOS))
  GetWidget()->GetNativeView()->AddPreTargetHandler(
      ConfirmQuitBubbleController::GetInstance());
#endif

#if defined(USE_AURA)
  SetThemeProfileForWindow(GetNativeWindow(), browser_->profile());
#endif

  LoadAccelerators();

  contents_web_view_ = new ContentsWebView(browser_->profile());
  contents_web_view_->set_id(VIEW_ID_TAB_CONTAINER);
  contents_web_view_->SetEmbedFullscreenWidgetMode(true);

  web_contents_close_handler_.reset(
      new WebContentsCloseHandler(contents_web_view_));

  devtools_web_view_ = new views::WebView(browser_->profile());
  devtools_web_view_->set_id(VIEW_ID_DEV_TOOLS_DOCKED);
  devtools_web_view_->SetVisible(false);

  contents_container_ = new views::View();
  contents_container_->SetBackground(views::CreateSolidBackground(
      GetThemeProvider()->GetColor(ThemeProperties::COLOR_CONTROL_BACKGROUND)));
  contents_container_->AddChildView(devtools_web_view_);
  contents_container_->AddChildView(contents_web_view_);
  contents_container_->SetLayoutManager(std::make_unique<ContentsLayoutManager>(
      devtools_web_view_, contents_web_view_));
  AddChildView(contents_container_);
  set_contents_view(contents_container_);

  top_container_ = new TopContainerView(this);
  AddChildView(top_container_);

  BrowserTabStripController* tabstrip_controller =
      new BrowserTabStripController(browser_->tab_strip_model(), this);
  tabstrip_ =
      new TabStrip(std::unique_ptr<TabStripController>(tabstrip_controller));
  top_container_->AddChildView(tabstrip_);   
  tabstrip_controller->InitFromModel(tabstrip_);

  toolbar_ = new ToolbarView(browser_.get(), this);
  top_container_->AddChildView(toolbar_);
  toolbar_->Init();

  if (!toolbar_button_provider_)
    SetToolbarButtonProvider(toolbar_);

  infobar_container_ = new InfoBarContainerView(this);
  AddChildView(infobar_container_);

  InitStatusBubble();

  find_bar_host_view_ = new View();
  AddChildView(find_bar_host_view_);

  immersive_mode_controller_->Init(this);

  auto browser_view_layout = std::make_unique<BrowserViewLayout>();
  browser_view_layout->Init(new BrowserViewLayoutDelegateImpl(this),
                            browser(),
                            this,
                            top_container_,
                            tabstrip_,
                            toolbar_,
                            infobar_container_,
                            contents_container_,
                            GetContentsLayoutManager(),
                            immersive_mode_controller_.get());
  SetLayoutManager(std::move(browser_view_layout));

#if defined(OS_WIN)
  if (JumpList::Enabled()) {
    load_complete_listener_.reset(new LoadCompleteListener(this));
  }
#endif

  frame_->OnBrowserViewInitViewsComplete();
  frame_->GetFrameView()->UpdateMinimumSize();
}
