RenderWidgetHostImpl::~RenderWidgetHostImpl() {
  if (!destroyed_)
    Destroy(false);
}
