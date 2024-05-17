void RenderWidgetHostImpl::ImeCancelComposition() {
  GetWidgetInputHandler()->ImeSetComposition(base::string16(),
                                             std::vector<ui::ImeTextSpan>(),
                                             gfx::Range::InvalidRange(), 0, 0);
}
