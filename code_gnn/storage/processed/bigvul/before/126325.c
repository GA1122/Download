void BrowserWindowGtk::DrawContentShadow(cairo_t* cr) {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  int left_x, top_y;
  gtk_widget_translate_coordinates(toolbar_->widget(),
      GTK_WIDGET(window_), 0, 0, &left_x,
      &top_y);

  GtkAllocation window_vbox_allocation;
  gtk_widget_get_allocation(window_vbox_, &window_vbox_allocation);
  int center_width = window_vbox_allocation.width;

  gfx::CairoCachedSurface* top_center =
      rb.GetNativeImageNamed(IDR_CONTENT_TOP_CENTER).ToCairo();
  gfx::CairoCachedSurface* top_right =
      rb.GetNativeImageNamed(IDR_CONTENT_TOP_RIGHT_CORNER).ToCairo();
  gfx::CairoCachedSurface* top_left =
      rb.GetNativeImageNamed(IDR_CONTENT_TOP_LEFT_CORNER).ToCairo();

  int center_left_x = left_x;
  if (ShouldDrawContentDropShadow()) {
    center_left_x += top_left->Width() - kContentShadowThickness;
    center_width -= (top_left->Width() + top_right->Width());
    center_width += 2 * kContentShadowThickness;
  }

  top_center->SetSource(cr, GTK_WIDGET(window_),
                        center_left_x, top_y - kContentShadowThickness);
  cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
  cairo_rectangle(cr, center_left_x, top_y - kContentShadowThickness,
                  center_width, top_center->Height());
  cairo_fill(cr);

  if (!ShouldDrawContentDropShadow())
    return;

  int right_x = center_left_x + center_width;
  top_left->SetSource(cr, GTK_WIDGET(window_),
      left_x - kContentShadowThickness, top_y - kContentShadowThickness);
  cairo_rectangle(cr,
      left_x - kContentShadowThickness,
      top_y - kContentShadowThickness,
      top_left->Width(),
      top_left->Height());
  cairo_fill(cr);

  top_right->SetSource(cr, GTK_WIDGET(window_),
                       right_x, top_y - kContentShadowThickness);
  cairo_rectangle(cr,
      right_x,
      top_y - kContentShadowThickness,
      top_right->Width(),
      top_right->Height());
  cairo_fill(cr);

  int bottom_y;
  gtk_widget_translate_coordinates(window_vbox_,
      GTK_WIDGET(window_),
      0, window_vbox_allocation.height,
      NULL, &bottom_y);
  int side_y = top_y - kContentShadowThickness + top_right->Height();
  int side_height = bottom_y - side_y - 1;
  if (side_height > 0) {
    gfx::CairoCachedSurface* left =
        rb.GetNativeImageNamed(IDR_CONTENT_LEFT_SIDE).ToCairo();
    left->SetSource(cr, GTK_WIDGET(window_),
                    left_x - kContentShadowThickness, side_y);
    cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
    cairo_rectangle(cr,
        left_x - kContentShadowThickness,
        side_y,
        kContentShadowThickness,
        side_height);
    cairo_fill(cr);

    gfx::CairoCachedSurface* right =
        rb.GetNativeImageNamed(IDR_CONTENT_RIGHT_SIDE).ToCairo();
    int right_side_x =
        right_x + top_right->Width() - kContentShadowThickness - 1;
    right->SetSource(cr, GTK_WIDGET(window_), right_side_x, side_y);
    cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
    cairo_rectangle(cr,
        right_side_x,
        side_y,
        kContentShadowThickness,
        side_height);
    cairo_fill(cr);
  }

  gfx::CairoCachedSurface* bottom_left =
      rb.GetNativeImageNamed(IDR_CONTENT_BOTTOM_LEFT_CORNER).ToCairo();
  bottom_left->SetSource(cr, GTK_WIDGET(window_),
                         left_x - kContentShadowThickness, bottom_y - 1);
  cairo_paint(cr);

  gfx::CairoCachedSurface* bottom_right =
      rb.GetNativeImageNamed(IDR_CONTENT_BOTTOM_RIGHT_CORNER).ToCairo();
  bottom_right->SetSource(cr, GTK_WIDGET(window_), right_x - 1, bottom_y - 1);
  cairo_paint(cr);

  gfx::CairoCachedSurface* bottom =
      rb.GetNativeImageNamed(IDR_CONTENT_BOTTOM_CENTER).ToCairo();
  bottom->SetSource(cr, GTK_WIDGET(window_), left_x + 1, bottom_y - 1);
  cairo_pattern_set_extend(cairo_get_source(cr), CAIRO_EXTEND_REPEAT);
  cairo_rectangle(cr,
      left_x + 1,
      bottom_y,
      window_vbox_allocation.width - 2,
      kContentShadowThickness);
  cairo_fill(cr);
}
