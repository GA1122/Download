gfx::NativeViewId RenderWidgetHostViewGtk::GetNativeViewId() const {
  return GtkNativeViewManager::GetInstance()->GetIdForWidget(view_.get());
}
