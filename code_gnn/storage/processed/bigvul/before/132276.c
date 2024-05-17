void RenderFrameImpl::WasShown() {
  if (render_widget_) {
    static_cast<blink::WebFrameWidget*>(render_widget_->webwidget())->
        setVisibilityState(blink::WebPageVisibilityStateVisible, false);
  }
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, WasShown());
}
