void StatusBubbleGtk::SetStatusTextToURL() {
  GtkWidget* parent = gtk_widget_get_parent(container_.get());

  if (!parent || !GTK_WIDGET_REALIZED(parent))
    return;

  int desired_width = parent->allocation.width;
  if (!expanded()) {
    expand_timer_.Stop();
    expand_timer_.Start(base::TimeDelta::FromMilliseconds(kExpandHoverDelay),
                        this, &StatusBubbleGtk::ExpandURL);
    desired_width /= 3;
  }

  url_text_ = UTF16ToUTF8(ui::ElideUrl(url_, gfx::Font(), desired_width,
                          UTF16ToWideHack(languages_)));
  SetStatusTextTo(url_text_);
}
