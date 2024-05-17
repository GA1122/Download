std::unique_ptr<TracedValue> InspectorPaintImageEvent::Data(
    const LayoutImage& layout_image,
    const FloatRect& src_rect,
    const FloatRect& dest_rect) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  SetGeneratingNodeInfo(value.get(), &layout_image, "nodeId");
  if (const ImageResourceContent* resource = layout_image.CachedImage())
    value->SetString("url", resource->Url().GetString());

  value->SetInteger("x", dest_rect.X());
  value->SetInteger("y", dest_rect.Y());
  value->SetInteger("width", dest_rect.Width());
  value->SetInteger("height", dest_rect.Height());
  value->SetInteger("srcWidth", src_rect.Width());
  value->SetInteger("srcHeight", src_rect.Height());

  return value;
}
