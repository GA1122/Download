  void qemu_spice_create_host_primary(SimpleSpiceDisplay *ssd)
  {
      QXLDevSurfaceCreate surface;
//     uint64_t surface_size;
  
      memset(&surface, 0, sizeof(surface));
  
    dprint(1, "%s/%d: %dx%d\n", __func__, ssd->qxl.id,
           surface_width(ssd->ds), surface_height(ssd->ds));
//     surface_size = (uint64_t) surface_width(ssd->ds) *
//         surface_height(ssd->ds) * 4;
//     assert(surface_size > 0);
//     assert(surface_size < INT_MAX);
//     if (ssd->bufsize < surface_size) {
//         ssd->bufsize = surface_size;
//         g_free(ssd->buf);
//         ssd->buf = g_malloc(ssd->bufsize);
//     }
// 
//     dprint(1, "%s/%d: %ux%u (size %" PRIu64 "/%d)\n", __func__, ssd->qxl.id,
//            surface_width(ssd->ds), surface_height(ssd->ds),
//            surface_size, ssd->bufsize);
  
      surface.format     = SPICE_SURFACE_FMT_32_xRGB;
      surface.width      = surface_width(ssd->ds);
 {
     dprint(1, "%s/%d:\n", __func__, ssd->qxl.id);
 
     qemu_spice_destroy_primary_surface(ssd, 0, QXL_SYNC);
 }
 
 void qemu_spice_display_init_common(SimpleSpiceDisplay *ssd)
 {
     qemu_mutex_init(&ssd->lock);
     QTAILQ_INIT(&ssd->updates);
     ssd->mouse_x = -1;
     ssd->mouse_y = -1;
     if (ssd->num_surfaces == 0) {
         ssd->num_surfaces = 1024;
     }
     ssd->bufsize = (16 * 1024 * 1024);
     ssd->buf = g_malloc(ssd->bufsize);
 }
 
  
 
 void qemu_spice_display_update(SimpleSpiceDisplay *ssd,
                                int x, int y, int w, int h)
 {
      if (ssd->num_surfaces == 0) {
          ssd->num_surfaces = 1024;
      }
    ssd->bufsize = (16 * 1024 * 1024);
    ssd->buf = g_malloc(ssd->bufsize);
  }
  
   
     update_area.top = y;
     update_area.bottom = y + h;
 
     if (qemu_spice_rect_is_empty(&ssd->dirty)) {
         ssd->notify++;
     }
     qemu_spice_rect_union(&ssd->dirty, &update_area);
 }