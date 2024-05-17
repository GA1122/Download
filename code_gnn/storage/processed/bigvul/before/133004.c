void RenderWidgetHostViewAura::PrepareBitmapCopyOutputResult(
    const gfx::Size& dst_size_in_pixel,
    const base::Callback<void(bool, const SkBitmap&)>& callback,
    scoped_ptr<cc::CopyOutputResult> result) {
  DCHECK(result->HasBitmap());

  base::ScopedClosureRunner scoped_callback_runner(
      base::Bind(callback, false, SkBitmap()));
  if (!result->HasBitmap())
    return;

  scoped_ptr<SkBitmap> source = result->TakeBitmap();
  DCHECK(source);
  if (!source)
    return;

  ignore_result(scoped_callback_runner.Release());

  SkBitmap bitmap = skia::ImageOperations::Resize(
      *source,
      skia::ImageOperations::RESIZE_BEST,
      dst_size_in_pixel.width(),
      dst_size_in_pixel.height());
  callback.Run(true, bitmap);
}
