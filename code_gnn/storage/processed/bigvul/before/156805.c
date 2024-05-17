bool Document::HaveRenderBlockingResourcesLoaded() const {
  if (RuntimeEnabledFeatures::CSSInBodyDoesNotBlockPaintEnabled()) {
    return HaveImportsLoaded() &&
           style_engine_->HaveRenderBlockingStylesheetsLoaded();
  }
  return HaveImportsLoaded() &&
         style_engine_->HaveScriptBlockingStylesheetsLoaded();
}
