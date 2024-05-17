void BrowserWindowGtk::DrawCustomFrame(cairo_t* cr,
                                       GtkWidget* widget,
                                       GdkEventExpose* event) {
  GtkThemeService* theme_provider = GtkThemeService::GetFrom(
      browser()->profile());

  int image_name = GetThemeFrameResource();

  gfx::CairoCachedSurface* surface = theme_provider->GetImageNamed(
      image_name).ToCairo();
  if (event->area.y < surface->Height()) {
    surface->SetSource(cr, widget, 0, GetVerticalOffset());

    cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
    cairo_rectangle(cr, event->area.x, event->area.y,
                    event->area.width, surface->Height() - event->area.y);
    cairo_fill(cr);
  }

  if (theme_provider->HasCustomImage(IDR_THEME_FRAME_OVERLAY) &&
      !browser()->profile()->IsOffTheRecord()) {
    gfx::CairoCachedSurface* theme_overlay = theme_provider->GetImageNamed(
        DrawFrameAsActive() ? IDR_THEME_FRAME_OVERLAY
        : IDR_THEME_FRAME_OVERLAY_INACTIVE).ToCairo();
    theme_overlay->SetSource(cr, widget, 0, GetVerticalOffset());
    cairo_paint(cr);
  }
}
