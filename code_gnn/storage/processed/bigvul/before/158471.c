  gfx::Rect TransformRectToViewsRootCoordSpace(const gfx::Rect rect,
                                               RenderWidgetHostView* view) {
    return gfx::Rect(view->TransformPointToRootCoordSpace(rect.origin()),
                     rect.size());
  }
