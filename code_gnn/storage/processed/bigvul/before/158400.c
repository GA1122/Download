  void DestroyView(FakeRenderWidgetHostViewAura* view) {
    bool is_guest_view_hack = view->is_guest_view_hack();
    RenderWidgetHostImpl* host = view->host();
    if (!is_guest_view_hack)
      EXPECT_EQ(view, host->GetView());
    view->Destroy();
    if (!is_guest_view_hack)
      EXPECT_EQ(nullptr, host->GetView());

    if (widget_host_uses_shutdown_to_destroy_)
      host->ShutdownAndDestroyWidget(true);
    else
      delete host;
  }
