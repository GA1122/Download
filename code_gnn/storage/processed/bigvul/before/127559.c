void BackingStoreGtk::XShowRect(const gfx::Point &origin,
                                const gfx::Rect& rect, XID target) {
  XCopyArea(display_, pixmap_, target, static_cast<GC>(pixmap_gc_),
            rect.x(), rect.y(), rect.width(), rect.height(),
            rect.x() + origin.x(), rect.y() + origin.y());
}
