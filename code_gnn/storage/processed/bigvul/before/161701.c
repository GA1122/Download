  explicit VaapiH264Picture(
      scoped_refptr<VaapiVideoDecodeAccelerator::VaapiDecodeSurface> surface)
      : dec_surface_(surface) {}
