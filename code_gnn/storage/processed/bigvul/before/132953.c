gfx::NativeViewId RenderWidgetHostViewAura::GetParentForWindowlessPlugin()
    const {
  return reinterpret_cast<gfx::NativeViewId>(plugin_parent_window_);
}
