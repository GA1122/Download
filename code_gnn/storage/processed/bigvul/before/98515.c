gboolean AutocompletePopupViewGtk::HandleExpose(GtkWidget* widget,
                                                GdkEventExpose* event) {
  bool ltr = !base::i18n::IsRTL();
  const AutocompleteResult& result = model_->result();

  gfx::Rect window_rect = GetWindowRect(event->window);
  gfx::Rect damage_rect = gfx::Rect(event->area);
  if (window_rect.width() < (kIconAreaWidth * 3))
    return TRUE;

  GdkDrawable* drawable = GDK_DRAWABLE(event->window);
  GdkGC* gc = gdk_gc_new(drawable);

  gdk_gc_set_rgb_fg_color(gc, &border_color_);

  COMPILE_ASSERT(kBorderThickness == 1, border_1px_implied);
  gdk_draw_rectangle(drawable, gc, FALSE,
                     0, 0,
                     window_rect.width() - 1, window_rect.height() - 1);

  pango_layout_set_height(layout_, kHeightPerResult * PANGO_SCALE);

  int gtk_offset = 0;
  if (theme_provider_->UseGtkTheme())
    gtk_offset = kGtkHorizontalOffset;

  for (size_t i = 0; i < result.size(); ++i) {
    gfx::Rect line_rect = GetRectForLine(i, window_rect.width());
    if (!line_rect.Intersects(damage_rect))
      continue;

    const AutocompleteMatch& match = result.match_at(i);
    bool is_selected = (model_->selected_line() == i);
    bool is_hovered = (model_->hovered_line() == i);
    if (is_selected || is_hovered) {
      gdk_gc_set_rgb_fg_color(gc, is_selected ? &selected_background_color_ :
                              &hovered_background_color_);
      gdk_draw_rectangle(drawable, gc, TRUE,
                         line_rect.x(), line_rect.y(),
                         line_rect.width(), line_rect.height());
    }

    int icon_start_x = ltr ? (kIconLeftPadding - gtk_offset) :
        (line_rect.width() - kIconLeftPadding - kIconWidth + gtk_offset);
    DrawFullPixbuf(drawable, gc,
                   IconForMatch(theme_provider_, match, is_selected),
                   icon_start_x, line_rect.y() + kIconTopPadding);

    bool has_description = !match.description.empty();
    int text_width = window_rect.width() - (kIconAreaWidth + kRightPadding);
    int allocated_content_width = has_description ?
        static_cast<int>(text_width * kContentWidthPercentage) : text_width;
    pango_layout_set_width(layout_, allocated_content_width * PANGO_SCALE);

    SetupLayoutForMatch(layout_, match.contents, match.contents_class,
                        is_selected ? &selected_content_text_color_ :
                        &content_text_color_,
                        is_selected ? &url_selected_text_color_ :
                            &url_text_color_,
                        std::string());

    int actual_content_width, actual_content_height;
    pango_layout_get_size(layout_,
        &actual_content_width, &actual_content_height);
    actual_content_width /= PANGO_SCALE;
    actual_content_height /= PANGO_SCALE;

    int content_y = std::max(line_rect.y(),
        line_rect.y() + ((kHeightPerResult - actual_content_height) / 2));

    gdk_draw_layout(drawable, gc,
                    ltr ? (kIconAreaWidth - gtk_offset) :
                        (text_width - actual_content_width + gtk_offset),
                    content_y, layout_);

    if (has_description) {
      pango_layout_set_width(layout_,
          (text_width - actual_content_width) * PANGO_SCALE);
      SetupLayoutForMatch(layout_, match.description, match.description_class,
                          is_selected ? &description_selected_text_color_ :
                              &description_text_color_,
                          is_selected ? &url_selected_text_color_ :
                              &url_text_color_,
                          std::string(" - "));
      gint actual_description_width;
      pango_layout_get_size(layout_, &actual_description_width, NULL);
      gdk_draw_layout(drawable, gc, ltr ?
                          (kIconAreaWidth - gtk_offset + actual_content_width) :
                          (text_width - actual_content_width + gtk_offset -
                           (actual_description_width / PANGO_SCALE)),
                      content_y, layout_);
    }
  }

  g_object_unref(gc);

  return TRUE;
}
