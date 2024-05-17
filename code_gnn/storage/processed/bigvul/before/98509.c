void DrawFullPixbuf(GdkDrawable* drawable, GdkGC* gc, GdkPixbuf* pixbuf,
                    gint dest_x, gint dest_y) {
  gdk_draw_pixbuf(drawable, gc, pixbuf,
                  0, 0,                         
                  dest_x, dest_y,               
                  -1, -1,                       
                  GDK_RGB_DITHER_NONE, 0, 0);   
}
