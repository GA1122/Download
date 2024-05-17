  FakeRenderWidgetHostViewAura* CreateView(bool is_guest_view_hack) {
    int32_t routing_id = process_host_->GetNextRoutingID();
    delegates_.push_back(base::WrapUnique(new MockRenderWidgetHostDelegate));
    auto* widget_host = MockRenderWidgetHostImpl::Create(
        delegates_.back().get(), process_host_, routing_id);
    delegates_.back()->set_widget_host(widget_host);
    widget_host->Init();
    return new FakeRenderWidgetHostViewAura(widget_host, is_guest_view_hack);
  }
