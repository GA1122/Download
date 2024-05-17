void RenderWidgetHostViewGtk::SetTooltipText(const string16& tooltip_text) {
  const int kMaxTooltipLength = 8 << 10;
  const string16 clamped_tooltip =
      ui::TruncateString(tooltip_text, kMaxTooltipLength);

  if (clamped_tooltip.empty()) {
    gtk_widget_set_has_tooltip(view_.get(), FALSE);
  } else {
    gtk_widget_set_tooltip_text(view_.get(),
                                UTF16ToUTF8(clamped_tooltip).c_str());
  }
}
