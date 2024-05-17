void RenderWidgetHostViewAura::CopyFromCompositingSurfaceHasResult(
    const gfx::Size& dst_size_in_pixel,
    bool readback_config_rgb565,
    const base::Callback<void(bool, const SkBitmap&)>& callback,
    scoped_ptr<cc::CopyOutputResult> result) {
  if (result->IsEmpty() || result->size().IsEmpty()) {
    callback.Run(false, SkBitmap());
    return;
  }

  if (result->HasTexture()) {
    PrepareTextureCopyOutputResult(dst_size_in_pixel, readback_config_rgb565,
                                   callback,
                                   result.Pass());
    return;
  }

  DCHECK(result->HasBitmap());
  PrepareBitmapCopyOutputResult(dst_size_in_pixel, callback, result.Pass());
}
