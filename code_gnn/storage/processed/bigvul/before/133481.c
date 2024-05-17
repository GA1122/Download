int32_t PepperFlashRendererHost::OnDrawGlyphs(
    ppapi::host::HostMessageContext* host_context,
    ppapi::proxy::PPBFlash_DrawGlyphs_Params params) {
  if (params.glyph_indices.size() != params.glyph_advances.size() ||
      params.glyph_indices.empty())
    return PP_ERROR_FAILED;

  int style = SkTypeface::kNormal;
  if (static_cast<PP_BrowserFont_Trusted_Weight>(params.font_desc.weight) >=
      PP_BROWSERFONT_TRUSTED_WEIGHT_BOLD)
    style |= SkTypeface::kBold;
  if (params.font_desc.italic)
    style |= SkTypeface::kItalic;
  skia::RefPtr<SkTypeface> typeface = skia::AdoptRef(
      SkTypeface::CreateFromName(params.font_desc.face.c_str(),
                                 static_cast<SkTypeface::Style>(style)));
  if (!typeface)
    return PP_ERROR_FAILED;

  EnterResourceNoLock<PPB_ImageData_API> enter(
      params.image_data.host_resource(), true);
  if (enter.failed())
    return PP_ERROR_FAILED;

  PPB_ImageData_API* image = static_cast<PPB_ImageData_API*>(
      enter.object());
  SkCanvas* canvas = image->GetPlatformCanvas();
  bool needs_unmapping = false;
  if (!canvas) {
    needs_unmapping = true;
    image->Map();
    canvas = image->GetPlatformCanvas();
    if (!canvas)
      return PP_ERROR_FAILED;   
  }

  SkAutoCanvasRestore acr(canvas, true);

  SkRect clip_rect = {
    SkIntToScalar(params.clip.point.x),
    SkIntToScalar(params.clip.point.y),
    SkIntToScalar(params.clip.point.x + params.clip.size.width),
    SkIntToScalar(params.clip.point.y + params.clip.size.height)
  };
  canvas->clipRect(clip_rect);

  SkMatrix matrix;
  matrix.set(SkMatrix::kMScaleX, SkFloatToScalar(params.transformation[0][0]));
  matrix.set(SkMatrix::kMSkewX,  SkFloatToScalar(params.transformation[0][1]));
  matrix.set(SkMatrix::kMTransX, SkFloatToScalar(params.transformation[0][2]));
  matrix.set(SkMatrix::kMSkewY,  SkFloatToScalar(params.transformation[1][0]));
  matrix.set(SkMatrix::kMScaleY, SkFloatToScalar(params.transformation[1][1]));
  matrix.set(SkMatrix::kMTransY, SkFloatToScalar(params.transformation[1][2]));
  matrix.set(SkMatrix::kMPersp0, SkFloatToScalar(params.transformation[2][0]));
  matrix.set(SkMatrix::kMPersp1, SkFloatToScalar(params.transformation[2][1]));
  matrix.set(SkMatrix::kMPersp2, SkFloatToScalar(params.transformation[2][2]));
  canvas->concat(matrix);

  SkPaint paint;
  paint.setColor(params.color);
  paint.setTextEncoding(SkPaint::kGlyphID_TextEncoding);
  paint.setAntiAlias(true);
  paint.setHinting(SkPaint::kFull_Hinting);
  paint.setTextSize(SkIntToScalar(params.font_desc.size));
  paint.setTypeface(typeface.get());   
  if (params.allow_subpixel_aa) {
    paint.setSubpixelText(true);
    paint.setLCDRenderText(true);
  }

  SkScalar x = SkIntToScalar(params.position.x);
  SkScalar y = SkIntToScalar(params.position.y);

  size_t glyph_count = params.glyph_indices.size();
  if (glyph_count) {
    std::vector<SkPoint> storage;
    storage.resize(glyph_count);
    SkPoint* sk_positions = &storage[0];
    for (uint32_t i = 0; i < glyph_count; i++) {
      sk_positions[i].set(x, y);
      x += SkFloatToScalar(params.glyph_advances[i].x);
      y += SkFloatToScalar(params.glyph_advances[i].y);
    }

    canvas->drawPosText(&params.glyph_indices[0], glyph_count * 2, sk_positions,
                        paint);
  }

  if (needs_unmapping)
    image->Unmap();

  return PP_OK;
}
