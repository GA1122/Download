webkit::ppapi::PluginInstance* RenderViewImpl::GetBitmapForOptimizedPluginPaint(
    const gfx::Rect& paint_bounds,
    TransportDIB** dib,
    gfx::Rect* location,
    gfx::Rect* clip) {
  return pepper_delegate_.GetBitmapForOptimizedPluginPaint(
      paint_bounds, dib, location, clip);
}
