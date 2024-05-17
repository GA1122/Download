void RenderWidgetHostViewGtk::CopyFromCompositingSurface(
    const gfx::Rect& src_subrect,
    const gfx::Size&  ,
    const base::Callback<void(bool)>& callback,
    skia::PlatformBitmap* output) {
  base::ScopedClosureRunner scoped_callback_runner(base::Bind(callback, false));

  gfx::Rect src_subrect_in_view = src_subrect;
  src_subrect_in_view.Offset(GetViewBounds().OffsetFromOrigin());

  ui::XScopedImage image(XGetImage(ui::GetXDisplay(), ui::GetX11RootWindow(),
                                   src_subrect_in_view.x(),
                                   src_subrect_in_view.y(),
                                   src_subrect_in_view.width(),
                                   src_subrect_in_view.height(),
                                   AllPlanes, ZPixmap));
  if (!image.get())
    return;

  if (!output->Allocate(src_subrect.width(), src_subrect.height(), true))
    return;

  const SkBitmap& bitmap = output->GetBitmap();
  const size_t bitmap_size = bitmap.getSize();
  DCHECK_EQ(bitmap_size,
            static_cast<size_t>(image->height * image->bytes_per_line));
  unsigned char* pixels = static_cast<unsigned char*>(bitmap.getPixels());
  memcpy(pixels, image->data, bitmap_size);

  scoped_callback_runner.Release();
  callback.Run(true);
}
