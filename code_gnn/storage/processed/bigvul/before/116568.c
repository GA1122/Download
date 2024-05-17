  bool is_showing() const {
    return static_cast<content::TestRenderWidgetHostView*>(
        GetRenderViewHostForTesting()->GetView())->is_showing();
  }
