  gfx::SelectionBound GetSelectionBoundFromRect(const gfx::Rect& rect) {
    gfx::SelectionBound bound;
    bound.SetEdge(gfx::PointF(rect.origin()), gfx::PointF(rect.bottom_left()));
    return bound;
  }
