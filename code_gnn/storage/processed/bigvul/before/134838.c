  RenderWidgetHostViewPort* GetRenderWidgetHostViewPort() const {
    RenderWidgetHostViewPort* const view =
        RenderWidgetHostViewPort::FromRWHV(GetRenderViewHost()->GetView());
    CHECK(view);
    return view;
  }
