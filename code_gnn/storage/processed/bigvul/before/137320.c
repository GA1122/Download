void Textfield::WriteDragDataForView(View* sender,
                                     const gfx::Point& press_pt,
                                     OSExchangeData* data) {
  const base::string16& selected_text(GetSelectedText());
  data->SetString(selected_text);
  Label label(selected_text, {GetFontList()});
  label.SetBackgroundColor(GetBackgroundColor());
  label.SetSubpixelRenderingEnabled(false);
  gfx::Size size(label.GetPreferredSize());
  gfx::NativeView native_view = GetWidget()->GetNativeView();
  display::Display display =
      display::Screen::GetScreen()->GetDisplayNearestView(native_view);
  size.SetToMin(gfx::Size(display.size().width(), height()));
  label.SetBoundsRect(gfx::Rect(size));
  label.SetEnabledColor(GetTextColor());

  SkBitmap bitmap;
  float raster_scale = ScaleFactorForDragFromWidget(GetWidget());
  SkColor color = SK_ColorTRANSPARENT;
#if defined(USE_X11)
  if (!ui::XVisualManager::GetInstance()->ArgbVisualAvailable())
    color = GetBackgroundColor();
#endif
  label.Paint(PaintInfo::CreateRootPaintInfo(
      ui::CanvasPainter(&bitmap, label.size(), raster_scale, color,
                        GetWidget()->GetCompositor()->is_pixel_canvas())
          .context(),
      label.size()));
  const gfx::Vector2d kOffset(-15, 0);
  gfx::ImageSkia image(gfx::ImageSkiaRep(bitmap, raster_scale));
  data->provider().SetDragImage(image, kOffset);
  if (controller_)
    controller_->OnWriteDragData(data);
}
