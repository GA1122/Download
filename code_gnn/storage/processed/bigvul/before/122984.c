void RenderWidgetHostImpl::InitializeOverscrollController() {
  overscroll_controller_.reset(new OverscrollController(this));
}
