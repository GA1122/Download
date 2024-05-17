static FloatRect ComputeTransformReferenceBox(const SVGElement& element) {
  const LayoutObject& layout_object = *element.GetLayoutObject();
  const ComputedStyle& style = layout_object.StyleRef();
  if (style.TransformBox() == ETransformBox::kFillBox)
    return layout_object.ObjectBoundingBox();
  DCHECK_EQ(style.TransformBox(), ETransformBox::kViewBox);
  SVGLengthContext length_context(&element);
  FloatSize viewport_size;
  length_context.DetermineViewport(viewport_size);
  return FloatRect(FloatPoint(), viewport_size);
}
