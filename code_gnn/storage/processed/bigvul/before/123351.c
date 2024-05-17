RenderWidgetHostViewGtk::~RenderWidgetHostViewGtk() {
  UnlockMouse();
  set_last_mouse_down(NULL);
  view_.Destroy();
}
