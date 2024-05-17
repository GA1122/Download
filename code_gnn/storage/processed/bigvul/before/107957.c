void InfoBarBackground::Paint(gfx::Canvas* canvas, views::View* view) const {
  gradient_background_->Paint(canvas, view);

  canvas->FillRectInt(ResourceBundle::toolbar_separator_color, 0,
                      view->height() - kSeparatorLineHeight, view->width(),
                      kSeparatorLineHeight);
}
