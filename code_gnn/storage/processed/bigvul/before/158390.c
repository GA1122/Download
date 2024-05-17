RenderWidgetHostImpl::~RenderWidgetHostImpl() {
  render_frame_metadata_provider_.RemoveObserver(this);
  if (!destroyed_)
    Destroy(false);
}
