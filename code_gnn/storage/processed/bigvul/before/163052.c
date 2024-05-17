int ConvertHinting(gfx::FontRenderParams::Hinting hinting) {
  switch (hinting) {
    case gfx::FontRenderParams::HINTING_NONE:
      return 0;
    case gfx::FontRenderParams::HINTING_SLIGHT:
      return 1;
    case gfx::FontRenderParams::HINTING_MEDIUM:
      return 2;
    case gfx::FontRenderParams::HINTING_FULL:
      return 3;
  }
  NOTREACHED() << "Unexpected hinting value " << hinting;
  return 0;
}
