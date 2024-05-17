VASurface::VASurface(VASurfaceID va_surface_id,
                     const gfx::Size& size,
                     const ReleaseCB& release_cb)
    : va_surface_id_(va_surface_id), size_(size), release_cb_(release_cb) {
  DCHECK(!release_cb_.is_null());
}
