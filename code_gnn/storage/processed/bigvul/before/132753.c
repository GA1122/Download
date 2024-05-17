void ChromotingInstance::SetCursorShape(
    const protocol::CursorShapeInfo& cursor_shape) {
  if (IsCursorShapeEmpty(cursor_shape)) {
    PostChromotingMessage("unsetCursorShape", pp::VarDictionary());
    return;
  }

  const int kBytesPerPixel = sizeof(uint32_t);
  const size_t buffer_size =
      cursor_shape.height() * cursor_shape.width() * kBytesPerPixel;

  pp::VarArrayBuffer array_buffer(buffer_size);
  void* dst = array_buffer.Map();
  memcpy(dst, cursor_shape.data().data(), buffer_size);
  array_buffer.Unmap();

  pp::VarDictionary dictionary;
  dictionary.Set(pp::Var("width"), cursor_shape.width());
  dictionary.Set(pp::Var("height"), cursor_shape.height());
  dictionary.Set(pp::Var("hotspotX"), cursor_shape.hotspot_x());
  dictionary.Set(pp::Var("hotspotY"), cursor_shape.hotspot_y());
  dictionary.Set(pp::Var("data"), array_buffer);
  PostChromotingMessage("setCursorShape", dictionary);
}
