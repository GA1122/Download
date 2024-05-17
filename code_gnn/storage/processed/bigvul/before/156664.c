  RenderWidgetHostViewAndroid* GetRenderWidgetHostViewAndroid() {
    return static_cast<RenderWidgetHostViewAndroid*>(
        shell()->web_contents()->GetRenderWidgetHostView());
  }
