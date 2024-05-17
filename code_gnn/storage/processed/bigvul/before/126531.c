void TabStripGtk::Init() {
  model_->AddObserver(this);

  tabstrip_.Own(gtk_fixed_new());
  ViewIDUtil::SetID(tabstrip_.get(), VIEW_ID_TAB_STRIP);
  gtk_widget_set_size_request(tabstrip_.get(), 0,
                              TabGtk::GetMinimumUnselectedSize().height());
  gtk_widget_set_app_paintable(tabstrip_.get(), TRUE);
  gtk_drag_dest_set(tabstrip_.get(), GTK_DEST_DEFAULT_ALL,
                    NULL, 0,
                    static_cast<GdkDragAction>(
                        GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_LINK));
  static const int targets[] = { ui::TEXT_URI_LIST,
                                 ui::NETSCAPE_URL,
                                 ui::TEXT_PLAIN,
                                 -1 };
  ui::SetDestTargetList(tabstrip_.get(), targets);

  g_signal_connect(tabstrip_.get(), "map",
                   G_CALLBACK(OnMapThunk), this);
  g_signal_connect(tabstrip_.get(), "expose-event",
                   G_CALLBACK(OnExposeThunk), this);
  g_signal_connect(tabstrip_.get(), "size-allocate",
                   G_CALLBACK(OnSizeAllocateThunk), this);
  g_signal_connect(tabstrip_.get(), "drag-motion",
                   G_CALLBACK(OnDragMotionThunk), this);
  g_signal_connect(tabstrip_.get(), "drag-drop",
                   G_CALLBACK(OnDragDropThunk), this);
  g_signal_connect(tabstrip_.get(), "drag-leave",
                   G_CALLBACK(OnDragLeaveThunk), this);
  g_signal_connect(tabstrip_.get(), "drag-data-received",
                   G_CALLBACK(OnDragDataReceivedThunk), this);

  newtab_button_.reset(MakeNewTabButton());
  newtab_surface_bounds_.SetRect(0, 0, newtab_button_->SurfaceWidth(),
                                 newtab_button_->SurfaceHeight());

  gtk_widget_show_all(tabstrip_.get());

  bounds_ = GetInitialWidgetBounds(tabstrip_.get());

  if (drop_indicator_width == 0) {
    GdkPixbuf* drop_image = GetDropArrowImage(true)->ToGdkPixbuf();
    drop_indicator_width = gdk_pixbuf_get_width(drop_image);
    drop_indicator_height = gdk_pixbuf_get_height(drop_image);
  }

  registrar_.Add(this, chrome::NOTIFICATION_BROWSER_THEME_CHANGED,
                 content::Source<ThemeService>(theme_service_));
  theme_service_->InitThemesFor(this);

  ViewIDUtil::SetDelegateForWidget(widget(), this);
}
