void BrowserWindowGtk::InitWidgets() {
  ConnectHandlersToSignals();

  bounds_ = configure_bounds_ = restored_bounds_ =
      GetInitialWindowBounds(window_);

  window_vbox_ = gtk_vbox_new(FALSE, 0);
  gtk_widget_show(window_vbox_);

  global_menu_bar_.reset(new GlobalMenuBar(browser_.get()));
  gtk_container_add(GTK_CONTAINER(window_vbox_), global_menu_bar_->widget());

  window_container_ = gtk_alignment_new(0.0, 0.0, 1.0, 1.0);
  gtk_widget_set_name(window_container_, "chrome-custom-frame-border");
  gtk_widget_set_app_paintable(window_container_, TRUE);
  gtk_widget_set_double_buffered(window_container_, FALSE);
  gtk_widget_set_redraw_on_allocate(window_container_, TRUE);
  g_signal_connect(window_container_, "expose-event",
                   G_CALLBACK(OnCustomFrameExposeThunk), this);
  gtk_container_add(GTK_CONTAINER(window_container_), window_vbox_);

  tabstrip_.reset(new TabStripGtk(browser_->tab_strip_model(), this));
  tabstrip_->Init();

  titlebar_.reset(new BrowserTitlebar(this, window_));
  titlebar_->Init();

  gtk_box_pack_start(GTK_BOX(window_vbox_), titlebar_->widget(), FALSE, FALSE,
                     0);

  toolbar_.reset(new BrowserToolbarGtk(browser_.get(), this));
  toolbar_->Init(window_);
  gtk_box_pack_start(GTK_BOX(window_vbox_), toolbar_->widget(),
                     FALSE, FALSE, 0);
  g_signal_connect_after(toolbar_->widget(), "expose-event",
                         G_CALLBACK(OnExposeDrawInfobarBitsThunk), this);
  render_area_vbox_ = gtk_vbox_new(FALSE, 0);
  gtk_widget_set_name(render_area_vbox_, "chrome-render-area-vbox");
  render_area_floating_container_ = gtk_floating_container_new();
  gtk_container_add(GTK_CONTAINER(render_area_floating_container_),
                    render_area_vbox_);

  GtkWidget* location_icon = toolbar_->GetLocationBarView()->
      location_icon_widget();
  g_signal_connect(location_icon, "size-allocate",
                   G_CALLBACK(OnLocationIconSizeAllocateThunk), this);
  g_signal_connect_after(location_icon, "expose-event",
                         G_CALLBACK(OnExposeDrawInfobarBitsThunk), this);

  toolbar_border_ = gtk_event_box_new();
  gtk_box_pack_start(GTK_BOX(render_area_vbox_),
                     toolbar_border_, FALSE, FALSE, 0);
  gtk_widget_set_size_request(toolbar_border_, -1, 1);
  gtk_widget_set_no_show_all(toolbar_border_, TRUE);
  g_signal_connect_after(toolbar_border_, "expose-event",
                         G_CALLBACK(OnExposeDrawInfobarBitsThunk), this);

  if (IsToolbarSupported())
    gtk_widget_show(toolbar_border_);

  infobar_container_.reset(
      new InfoBarContainerGtk(this,
                              browser_->search_model(),
                              browser_->profile()));
  gtk_box_pack_start(GTK_BOX(render_area_vbox_),
                     infobar_container_->widget(),
                     FALSE, FALSE, 0);

  status_bubble_.reset(new StatusBubbleGtk(browser_->profile()));

  contents_container_.reset(new TabContentsContainerGtk(status_bubble_.get()));
  devtools_container_.reset(new TabContentsContainerGtk(NULL));
  ViewIDUtil::SetID(devtools_container_->widget(), VIEW_ID_DEV_TOOLS_DOCKED);

  contents_hsplit_ = gtk_hpaned_new();
  gtk_paned_pack1(GTK_PANED(contents_hsplit_), contents_container_->widget(),
                  TRUE, TRUE);
  contents_vsplit_ = gtk_vpaned_new();
  gtk_paned_pack1(GTK_PANED(contents_vsplit_), contents_hsplit_, TRUE, TRUE);

  gtk_box_pack_end(GTK_BOX(render_area_vbox_),
                   contents_vsplit_, TRUE, TRUE, 0);

  gtk_widget_show_all(render_area_floating_container_);
  render_area_event_box_ = gtk_event_box_new();
  gtk_widget_modify_bg(render_area_event_box_, GTK_STATE_NORMAL,
                       &ui::kGdkWhite);
  gtk_container_add(GTK_CONTAINER(render_area_event_box_),
                    render_area_floating_container_);
  gtk_widget_show(render_area_event_box_);
  gtk_box_pack_end(GTK_BOX(window_vbox_), render_area_event_box_,
                   TRUE, TRUE, 0);

  instant_preview_controller_.reset(
      new InstantPreviewControllerGtk(browser_.get(),
                                      this,
                                      contents_container_.get()));

  if (IsBookmarkBarSupported()) {
    bookmark_bar_.reset(new BookmarkBarGtk(this,
                                           browser_.get(),
                                           tabstrip_.get()));
    PlaceBookmarkBar(false);
    gtk_widget_show(bookmark_bar_->widget());

    g_signal_connect_after(bookmark_bar_->widget(), "expose-event",
                           G_CALLBACK(OnBookmarkBarExposeThunk), this);
    g_signal_connect(bookmark_bar_->widget(), "size-allocate",
                     G_CALLBACK(OnBookmarkBarSizeAllocateThunk), this);
  }

  gtk_widget_realize(GTK_WIDGET(window_));
  state_ = gdk_window_get_state(gtk_widget_get_window(GTK_WIDGET(window_)));
  UpdateCustomFrame();

  extension_keybinding_registry_.reset(new ExtensionKeybindingRegistryGtk(
      browser_->profile(),
      window_,
      extensions::ExtensionKeybindingRegistry::ALL_EXTENSIONS,
      this));

  static bool default_icon_set = false;
  if (!default_icon_set) {
    gtk_util::SetDefaultWindowIcon(window_);
    default_icon_set = true;
  }
  gtk_util::SetWindowIcon(window_, browser_->profile());

  gtk_container_add(GTK_CONTAINER(window_), window_container_);
  gtk_widget_show(window_container_);
  browser_->tab_strip_model()->AddObserver(this);
}
