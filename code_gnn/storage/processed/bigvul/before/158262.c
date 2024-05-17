void RenderWidgetHostImpl::Focus() {
  RenderWidgetHostImpl* focused_widget =
      delegate_ ? delegate_->GetRenderWidgetHostWithPageFocus() : nullptr;

  if (!focused_widget)
    focused_widget = this;
  focused_widget->SetPageFocus(true);
}
