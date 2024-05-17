  TestRenderWidgetHostView* CreateViewForProcess(
      MockRenderWidgetHostImpl* host) {
    TestRenderWidgetHostView* view = new TestRenderWidgetHostView(host);
    host->SetView(view);
    return view;
  }
