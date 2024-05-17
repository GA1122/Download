int ConvertSubpixelRendering(
    gfx::FontRenderParams::SubpixelRendering rendering) {
  switch (rendering) {
    case gfx::FontRenderParams::SUBPIXEL_RENDERING_NONE:
      return 0;
    case gfx::FontRenderParams::SUBPIXEL_RENDERING_RGB:
      return 1;
    case gfx::FontRenderParams::SUBPIXEL_RENDERING_BGR:
      return 1;
    case gfx::FontRenderParams::SUBPIXEL_RENDERING_VRGB:
      return 1;
    case gfx::FontRenderParams::SUBPIXEL_RENDERING_VBGR:
      return 1;
  }
  NOTREACHED() << "Unexpected subpixel rendering value " << rendering;
  return 0;
}
