bool Document::HaveRenderBlockingStylesheetsLoaded() const {
  if (RuntimeEnabledFeatures::CSSInBodyDoesNotBlockPaintEnabled())
    return style_engine_->HaveRenderBlockingStylesheetsLoaded();
  return style_engine_->HaveScriptBlockingStylesheetsLoaded();
}
