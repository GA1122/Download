void RenderWidgetHostImpl::Blur() {
  RenderWidgetHostImpl* focused_widget =
      delegate_ ? delegate_->GetRenderWidgetHostWithPageFocus() : nullptr;

  if (!focused_widget)
    focused_widget = this;
  focused_widget->SetPageFocus(false);
}
