std::unique_ptr<base::DictionaryValue> CreateBoundsDict(
    const HeadlessWebContentsImpl* web_contents) {
  auto bounds_object = base::MakeUnique<base::DictionaryValue>();
  gfx::Rect bounds = web_contents->web_contents()->GetContainerBounds();
  bounds_object->SetInteger("left", bounds.x());
  bounds_object->SetInteger("top", bounds.y());
  bounds_object->SetInteger("width", bounds.width());
  bounds_object->SetInteger("height", bounds.height());
  bounds_object->SetString("windowState", web_contents->window_state());
  return bounds_object;
}
