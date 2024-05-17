  void SetUpOverscrollEnvironmentImpl(int debounce_interval_in_ms) {
    SetFeatureList();
    scoped_feature_list_.InitAndEnableFeature(
        features::kTouchpadOverscrollHistoryNavigation);

    ui::GestureConfiguration::GetInstance()->set_scroll_debounce_interval_in_ms(
        debounce_interval_in_ms);

    RenderWidgetHostViewAuraTest::SetUpEnvironment();

    view_->SetOverscrollControllerEnabled(true);
    gfx::Size display_size = display::Screen::GetScreen()
                                 ->GetDisplayNearestView(view_->GetNativeView())
                                 .size();
    overscroll_delegate_.reset(new TestOverscrollDelegate(display_size));
    view_->overscroll_controller()->set_delegate(overscroll_delegate_.get());

    view_->InitAsChild(nullptr);
    view_->SetBounds(gfx::Rect(0, 0, 400, 200));
    view_->Show();

    sink_->ClearMessages();
  }
