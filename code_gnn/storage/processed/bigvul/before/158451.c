  void SetUpEnvironment() {
    ImageTransportFactory::SetFactory(
        std::make_unique<TestImageTransportFactory>());
    aura_test_helper_.reset(new aura::test::AuraTestHelper());
    aura_test_helper_->SetUp(
        ImageTransportFactory::GetInstance()->GetContextFactory(),
        ImageTransportFactory::GetInstance()->GetContextFactoryPrivate());
    new wm::DefaultActivationClient(aura_test_helper_->root_window());

    browser_context_.reset(new TestBrowserContext);
    process_host_ = new MockRenderProcessHost(browser_context_.get());
    process_host_->Init();

    sink_ = &process_host_->sink();

    int32_t routing_id = process_host_->GetNextRoutingID();
    delegates_.push_back(base::WrapUnique(new MockRenderWidgetHostDelegate));
    parent_host_ = MockRenderWidgetHostImpl::Create(delegates_.back().get(),
                                                    process_host_, routing_id);
    delegates_.back()->set_widget_host(parent_host_);
    const bool is_mus_browser_plugin_guest = false;
    parent_view_ = new RenderWidgetHostViewAura(
        parent_host_, is_guest_view_hack_, is_mus_browser_plugin_guest);
    parent_view_->InitAsChild(nullptr);
    aura::client::ParentWindowWithContext(parent_view_->GetNativeView(),
                                          aura_test_helper_->root_window(),
                                          gfx::Rect());
    view_ = CreateView(is_guest_view_hack_);
    widget_host_ = static_cast<MockRenderWidgetHostImpl*>(view_->host());
    view_->event_handler()->set_mouse_wheel_wheel_phase_handler_timeout(
        base::TimeDelta::FromMilliseconds(100));
    base::RunLoop().RunUntilIdle();
  }
