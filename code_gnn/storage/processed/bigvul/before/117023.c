gboolean OmniboxPopupViewGtk::HandleExpose(GtkWidget* widget,
                                           GdkEventExpose* event) {
  bool ltr = !base::i18n::IsRTL();
  const AutocompleteResult& result = model_->result();

  gfx::Rect window_rect = GetWindowRect(event->window);
  gfx::Rect damage_rect = gfx::Rect(event->area);
  if (window_rect.width() < (kIconAreaWidth * 3))
    return TRUE;

  cairo_t* cr = gdk_cairo_create(gtk_widget_get_window(widget));
  gdk_cairo_rectangle(cr, &event->area);
  cairo_clip(cr);

  COMPILE_ASSERT(kBorderThickness == 1, border_1px_implied);
  gdk_cairo_set_source_color(cr, &border_color_);
  cairo_rectangle(cr, 0, 0, window_rect.width(), window_rect.height());
  cairo_stroke(cr);

  pango_layout_set_height(layout_, kHeightPerResult * PANGO_SCALE);

  for (size_t i = 0; i < result.size(); ++i) {
    gfx::Rect line_rect = GetRectForLine(i, window_rect.width());
    if (!line_rect.Intersects(damage_rect))
      continue;

    const AutocompleteMatch* match = NULL;
    bool is_selected_keyword = false;
    GetVisibleMatchForInput(i, &match, &is_selected_keyword);
    bool is_selected = (model_->selected_line() == i);
    bool is_hovered = (model_->hovered_line() == i);
    if (is_selected || is_hovered) {
      gdk_cairo_set_source_color(cr, is_selected ? &selected_background_color_ :
                                 &hovered_background_color_);
      cairo_rectangle(cr, line_rect.x(), line_rect.y(),
                      line_rect.width(), line_rect.height());
      cairo_fill(cr);
    }

    int icon_start_x = ltr ? kIconLeftPadding :
        (line_rect.width() - kIconLeftPadding - kIconWidth);
    DrawFullImage(cr, widget,
                  IconForMatch(*match, is_selected, is_selected_keyword),
                  icon_start_x, line_rect.y() + kIconTopPadding);

    bool has_description = !match->description.empty();
    int text_width = window_rect.width() - (kIconAreaWidth + kRightPadding);
    int allocated_content_width = has_description ?
        static_cast<int>(text_width * kContentWidthPercentage) : text_width;
    pango_layout_set_width(layout_, allocated_content_width * PANGO_SCALE);

    SetupLayoutForMatch(layout_, match->contents, match->contents_class,
                        is_selected ? &selected_content_text_color_ :
                            &content_text_color_,
                        is_selected ? &selected_content_dim_text_color_ :
                            &content_dim_text_color_,
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

    cairo_save(cr);
    cairo_move_to(cr,
                  ltr ? kIconAreaWidth :
                        (text_width - actual_content_width),
                  content_y);
    pango_cairo_show_layout(cr, layout_);
    cairo_restore(cr);

    if (has_description) {
      pango_layout_set_width(layout_,
          (text_width - actual_content_width) * PANGO_SCALE);

      SetupLayoutForMatch(layout_, match->description, match->description_class,
                          is_selected ? &selected_content_dim_text_color_ :
                              &content_dim_text_color_,
                          is_selected ? &selected_content_dim_text_color_ :
                              &content_dim_text_color_,
                          is_selected ? &url_selected_text_color_ :
                              &url_text_color_,
                          std::string(" - "));
      gint actual_description_width;
      pango_layout_get_size(layout_, &actual_description_width, NULL);

      cairo_save(cr);
      cairo_move_to(cr, ltr ?
                    (kIconAreaWidth + actual_content_width) :
                    (text_width - actual_content_width -
                     (actual_description_width / PANGO_SCALE)),
                    content_y);
      pango_cairo_show_layout(cr, layout_);
      cairo_restore(cr);
    }

    if (match->associated_keyword.get()) {
      icon_start_x = ltr ? (line_rect.width() - kIconLeftPadding - kIconWidth) :
          kIconLeftPadding;
      DrawFullImage(cr, widget,
                    theme_service_->GetImageNamed(
                        is_selected ? IDR_OMNIBOX_TTS_DARK : IDR_OMNIBOX_TTS),
                    icon_start_x, line_rect.y() + kIconTopPadding);
    }
  }

  cairo_destroy(cr);
  return TRUE;
}
