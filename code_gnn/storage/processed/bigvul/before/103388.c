void CapturerMac::ScreenUpdateMove(CGScreenUpdateMoveDelta delta,
                                   size_t count,
                                   const CGRect *rect_array) {
  InvalidRects rects;
  for (CGRectCount i = 0; i < count; ++i) {
    CGRect rect = rect_array[i];
    rects.insert(gfx::Rect(rect));
    rect = CGRectOffset(rect, delta.dX, delta.dY);
    rects.insert(gfx::Rect(rect));
  }
  InvalidateRects(rects);
}
