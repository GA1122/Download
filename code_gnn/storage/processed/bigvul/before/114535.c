void WebPluginProxy::CreateDIBAndCGContextFromHandle(
    const TransportDIB::Handle& dib_handle,
    const gfx::Rect& window_rect,
    scoped_ptr<TransportDIB>* dib_out,
    base::mac::ScopedCFTypeRef<CGContextRef>* cg_context_out) {
  TransportDIB* dib = TransportDIB::Map(dib_handle);
  CGContextRef cg_context = NULL;
  if (dib) {
    cg_context = CGBitmapContextCreate(
        dib->memory(),
        window_rect.width(),
        window_rect.height(),
        8,
        4 * window_rect.width(),
        base::mac::GetSystemColorSpace(),
        kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);
    CGContextTranslateCTM(cg_context, 0, window_rect.height());
    CGContextScaleCTM(cg_context, 1, -1);
  }
  dib_out->reset(dib);
  cg_context_out->reset(cg_context);
}
