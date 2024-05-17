bool WebPluginDelegateProxy::CreateLocalBitmap(
    std::vector<uint8>* memory,
    scoped_ptr<skia::PlatformCanvas>* canvas) {
  const size_t size = BitmapSizeForPluginRect(plugin_rect_);
  memory->resize(size);
  if (memory->size() != size)
    return false;
  canvas->reset(new skia::PlatformCanvas(
      plugin_rect_.width(), plugin_rect_.height(), true, &((*memory)[0])));
  return true;
}
