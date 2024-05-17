BackingStoreGtk::BackingStoreGtk(RenderWidgetHost* widget,
                                 const gfx::Size& size,
                                 void* visual,
                                 int depth)
    : BackingStore(widget, size),
      display_(ui::GetXDisplay()),
      shared_memory_support_(ui::QuerySharedMemorySupport(display_)),
      use_render_(ui::QueryRenderSupport(display_)),
      visual_(visual),
      visual_depth_(depth),
      root_window_(ui::GetX11RootWindow()) {
#if defined(OS_OPENBSD) || defined(OS_FREEBSD)
  COMPILE_ASSERT(_BYTE_ORDER == _LITTLE_ENDIAN, assumes_little_endian);
#else
  COMPILE_ASSERT(__BYTE_ORDER == __LITTLE_ENDIAN, assumes_little_endian);
#endif

  pixmap_ = XCreatePixmap(display_, root_window_,
                          size.width(), size.height(), depth);

  if (use_render_) {
    picture_ = XRenderCreatePicture(
        display_, pixmap_,
        ui::GetRenderVisualFormat(display_,
                                  static_cast<Visual*>(visual)),
                                  0, NULL);
    pixmap_bpp_ = 0;
  } else {
    picture_ = 0;
    pixmap_bpp_ = ui::BitsPerPixelForPixmapDepth(display_, depth);
  }

  pixmap_gc_ = XCreateGC(display_, pixmap_, 0, NULL);
}
