std::unique_ptr<TracedValue> InspectorPaintImageEvent::Data(
    Node* node,
    const StyleImage& style_image,
    const FloatRect& src_rect,
    const FloatRect& dest_rect) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  if (node)
    SetNodeInfo(value.get(), node, "nodeId", nullptr);
  if (const ImageResourceContent* resource = style_image.CachedImage())
    value->SetString("url", resource->Url().GetString());

  value->SetInteger("x", dest_rect.X());
  value->SetInteger("y", dest_rect.Y());
  value->SetInteger("width", dest_rect.Width());
  value->SetInteger("height", dest_rect.Height());
  value->SetInteger("srcWidth", src_rect.Width());
  value->SetInteger("srcHeight", src_rect.Height());

  return value;
}
