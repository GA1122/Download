void BrowserView::Init() {
  GetWidget()->AddObserver(this);

  SetLayoutManager(CreateLayoutManager());
  GetWidget()->SetNativeWindowProperty(kBrowserViewKey, this);

  GetWidget()->SetNativeWindowProperty(Profile::kProfileKey,
                                       browser_->profile());

  if (!CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableHangMonitor)) {
    InitHangMonitor();
  }

  LoadAccelerators();

  BrowserTabStripController* tabstrip_controller =
      new BrowserTabStripController(browser_.get(),
                                    browser_->tab_strip_model());
  tabstrip_ = new TabStrip(tabstrip_controller);
  AddChildView(tabstrip_);
  tabstrip_controller->InitFromModel(tabstrip_);

  infobar_container_ = new InfoBarContainerView(this,
                                                browser()->search_model());
  AddChildView(infobar_container_);

  contents_container_ = new views::WebView(browser_->profile());
  contents_container_->set_id(VIEW_ID_TAB_CONTAINER);
  contents_ = new ContentsContainer(contents_container_);

  toolbar_ = new ToolbarView(browser_.get());
  AddChildView(toolbar_);
  toolbar_->Init();

  preview_controller_.reset(
      new InstantPreviewControllerViews(browser(), this, contents_));

  SkColor bg_color = GetWidget()->GetThemeProvider()->
      GetColor(ThemeService::COLOR_TOOLBAR);

  devtools_container_ = new views::WebView(browser_->profile());
  devtools_container_->set_id(VIEW_ID_DEV_TOOLS_DOCKED);
  devtools_container_->SetVisible(false);

  views::View* contents_view = contents_;

  contents_split_ = new views::SingleSplitView(
      contents_view,
      devtools_container_,
      views::SingleSplitView::VERTICAL_SPLIT,
      this);
  contents_split_->set_id(VIEW_ID_CONTENTS_SPLIT);
  contents_split_->SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_ACCNAME_WEB_CONTENTS));
  contents_split_->set_background(
      views::Background::CreateSolidBackground(bg_color));
  AddChildView(contents_split_);
  set_contents_view(contents_split_);

  status_bubble_.reset(new StatusBubbleViews(contents_));

#if defined(OS_WIN) && !defined(USE_AURA)
  if (JumpList::Enabled()) {
    jumplist_ = new JumpList();
    jumplist_->AddObserver(browser_->profile());
  }
#endif

  ignore_layout_ = false;
}
