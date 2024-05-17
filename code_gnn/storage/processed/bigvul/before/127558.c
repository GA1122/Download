void BackingStoreGtk::ScrollBackingStore(int dx, int dy,
                                         const gfx::Rect& clip_rect,
                                         const gfx::Size& view_size) {
  if (!display_)
    return;

  DCHECK(dx == 0 || dy == 0);

  if (dy) {
    if (abs(dy) < clip_rect.height()) {
      XCopyArea(display_, pixmap_, pixmap_, static_cast<GC>(pixmap_gc_),
                clip_rect.x()  ,
                std::max(clip_rect.y(), clip_rect.y() - dy),
                clip_rect.width(),
                clip_rect.height() - abs(dy),
                clip_rect.x()  ,
                std::max(clip_rect.y(), clip_rect.y() + dy)  );
    }
  } else if (dx) {
    if (abs(dx) < clip_rect.width()) {
      XCopyArea(display_, pixmap_, pixmap_, static_cast<GC>(pixmap_gc_),
                std::max(clip_rect.x(), clip_rect.x() - dx),
                clip_rect.y()  ,
                clip_rect.width() - abs(dx),
                clip_rect.height(),
                std::max(clip_rect.x(), clip_rect.x() + dx)  ,
                clip_rect.y()  );
    }
  }
}
