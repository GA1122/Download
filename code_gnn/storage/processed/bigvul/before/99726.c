VASurface::~VASurface() {
  release_cb_.Run(va_surface_id_);
}
