static FloatPoint PerspectiveOrigin(const LayoutBox& box) {
  const ComputedStyle& style = box.StyleRef();
  DCHECK(style.HasPerspective());
  FloatSize border_box_size(box.Size());
  return FloatPointForLengthPoint(style.PerspectiveOrigin(), border_box_size);
}
