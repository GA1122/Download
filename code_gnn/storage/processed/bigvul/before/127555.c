void BackingStoreGtk::PaintToBackingStore(
    RenderProcessHost* process,
    TransportDIB::Id bitmap,
    const gfx::Rect& bitmap_rect,
    const std::vector<gfx::Rect>& copy_rects,
    float scale_factor,
    const base::Closure& completion_callback,
    bool* scheduled_completion_callback) {
  *scheduled_completion_callback = false;

  if (!display_)
    return;

  if (bitmap_rect.IsEmpty())
    return;

  const int width = bitmap_rect.width();
  const int height = bitmap_rect.height();

  if (width <= 0 || width > kMaxVideoLayerSize ||
      height <= 0 || height > kMaxVideoLayerSize)
    return;

  TransportDIB* dib = process->GetTransportDIB(bitmap);
  if (!dib)
    return;

  if (!use_render_)
    return PaintRectWithoutXrender(dib, bitmap_rect, copy_rects);

  Picture picture;
  Pixmap pixmap;

  if (shared_memory_support_ == ui::SHARED_MEMORY_PIXMAP) {
    XShmSegmentInfo shminfo = {0};
    shminfo.shmseg = dib->MapToX(display_);

    pixmap = XShmCreatePixmap(display_, root_window_, NULL, &shminfo,
                              width, height, 32);
  } else {
    pixmap = XCreatePixmap(display_, root_window_, width, height, 32);
    GC gc = XCreateGC(display_, pixmap, 0, NULL);

    if (shared_memory_support_ == ui::SHARED_MEMORY_PUTIMAGE) {
      const XID shmseg = dib->MapToX(display_);

      XShmSegmentInfo shminfo;
      memset(&shminfo, 0, sizeof(shminfo));
      shminfo.shmseg = shmseg;
      shminfo.shmaddr = static_cast<char*>(dib->memory());

      XImage* image = XShmCreateImage(display_, static_cast<Visual*>(visual_),
                                      32, ZPixmap,
                                      shminfo.shmaddr, &shminfo,
                                      width, height);

#if defined(ARCH_CPU_ARM_FAMILY)
      for (size_t i = 0; i < copy_rects.size(); i++) {
        const gfx::Rect& copy_rect = copy_rects[i];
        XShmPutImage(display_, pixmap, gc, image,
                     copy_rect.x() - bitmap_rect.x(),  
                     copy_rect.y() - bitmap_rect.y(),  
                     copy_rect.x() - bitmap_rect.x(),  
                     copy_rect.y() - bitmap_rect.y(),  
                     copy_rect.width(), copy_rect.height(),
                     False  );
      }
#else
      XShmPutImage(display_, pixmap, gc, image,
                   0, 0  , 0, 0  ,
                   width, height, False  );
#endif
      XDestroyImage(image);
    } else {   
      XImage image;
      memset(&image, 0, sizeof(image));

      image.width = width;
      image.height = height;
      image.depth = 32;
      image.bits_per_pixel = 32;
      image.format = ZPixmap;
      image.byte_order = LSBFirst;
      image.bitmap_unit = 8;
      image.bitmap_bit_order = LSBFirst;
      image.bytes_per_line = width * 4;
      image.red_mask = 0xff;
      image.green_mask = 0xff00;
      image.blue_mask = 0xff0000;
      image.data = static_cast<char*>(dib->memory());

      XPutImage(display_, pixmap, gc, &image,
                0, 0  , 0, 0  ,
                width, height);
    }
    XFreeGC(display_, gc);
  }

  picture = ui::CreatePictureFromSkiaPixmap(display_, pixmap);

  for (size_t i = 0; i < copy_rects.size(); i++) {
    const gfx::Rect& copy_rect = copy_rects[i];
    XRenderComposite(display_,
                     PictOpSrc,                         
                     picture,                           
                     0,                                 
                     picture_,                          
                     copy_rect.x() - bitmap_rect.x(),   
                     copy_rect.y() - bitmap_rect.y(),   
                     0,                                 
                     0,                                 
                     copy_rect.x(),                     
                     copy_rect.y(),                     
                     copy_rect.width(),                 
                     copy_rect.height());               
  }

  if (shared_memory_support_ != ui::SHARED_MEMORY_NONE) {
    XSyncHandler* handler = XSyncHandler::GetInstance();
    if (handler->Enabled()) {
      *scheduled_completion_callback = true;
      handler->PushPaintCounter(display_, picture, pixmap, completion_callback);
    } else {
      XSync(display_, False);
    }
  }

  if (*scheduled_completion_callback == false) {
    XRenderFreePicture(display_, picture);
    XFreePixmap(display_, pixmap);
  }
}
