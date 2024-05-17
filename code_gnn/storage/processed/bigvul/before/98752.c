WebPluginDelegatePepper::~WebPluginDelegatePepper() {
  DestroyInstance();

  if (render_view_)
    render_view_->OnPepperPluginDestroy(this);
}
