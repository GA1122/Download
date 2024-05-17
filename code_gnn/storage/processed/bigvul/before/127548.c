BackingStoreGtk::BackingStoreGtk(RenderWidgetHost* widget,
                                 const gfx::Size& size)
    : BackingStore(widget, size),
      display_(NULL),
      shared_memory_support_(ui::SHARED_MEMORY_NONE),
      use_render_(false),
      pixmap_bpp_(0),
      visual_(NULL),
      visual_depth_(-1),
      root_window_(0),
      pixmap_(0),
      picture_(0),
      pixmap_gc_(NULL) {
}
