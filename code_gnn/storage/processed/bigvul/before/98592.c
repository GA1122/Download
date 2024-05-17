DraggedTabGtk::DraggedTabGtk(TabContents* datasource,
                             const gfx::Point& mouse_tab_offset,
                             const gfx::Size& contents_size,
                             bool mini)
    : data_source_(datasource),
      renderer_(new TabRendererGtk(datasource->profile()->GetThemeProvider())),
      attached_(false),
      mouse_tab_offset_(mouse_tab_offset),
      attached_tab_size_(TabRendererGtk::GetMinimumSelectedSize()),
      contents_size_(contents_size),
      close_animation_(this) {
  renderer_->UpdateData(datasource, false, false);
  renderer_->set_mini(mini);

  container_ = gtk_window_new(GTK_WINDOW_POPUP);
  SetContainerColorMap();
  gtk_widget_set_app_paintable(container_, TRUE);
  g_signal_connect(container_, "expose-event",
                   G_CALLBACK(OnExposeEvent), this);
  gtk_widget_add_events(container_, GDK_STRUCTURE_MASK);

  fixed_ = gtk_fixed_new();
  gtk_fixed_put(GTK_FIXED(fixed_), renderer_->widget(), 0, 0);
  gtk_container_add(GTK_CONTAINER(container_), fixed_);
  gtk_widget_show_all(container_);
}
