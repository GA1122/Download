void BackingStoreGtk::PaintRectWithoutXrender(
    TransportDIB* bitmap,
    const gfx::Rect& bitmap_rect,
    const std::vector<gfx::Rect>& copy_rects) {
  const int width = bitmap_rect.width();
  const int height = bitmap_rect.height();
  Pixmap pixmap = XCreatePixmap(display_, root_window_, width, height,
                                visual_depth_);

  ui::PutARGBImage(display_, visual_, visual_depth_, pixmap,
                   pixmap_gc_, static_cast<uint8*>(bitmap->memory()),
                   width, height);

  for (size_t i = 0; i < copy_rects.size(); i++) {
    const gfx::Rect& copy_rect = copy_rects[i];
    XCopyArea(display_,
              pixmap,                            
              pixmap_,                           
              static_cast<GC>(pixmap_gc_),       
              copy_rect.x() - bitmap_rect.x(),   
              copy_rect.y() - bitmap_rect.y(),   
              copy_rect.width(),                 
              copy_rect.height(),                
              copy_rect.x(),                     
              copy_rect.y());                    
  }

  XFreePixmap(display_, pixmap);
}
