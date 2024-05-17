static LayoutRect AbsoluteToLocal(const LayoutBox& box, LayoutRect rect) {
  return LayoutRect(
      box.AbsoluteToLocalQuad(FloatQuad(FloatRect(rect)), kUseTransforms)
          .BoundingBox());
}
