bool Document::HaveRenderBlockingResourcesLoaded() const {
  return HaveImportsLoaded() &&
         style_engine_->HaveRenderBlockingStylesheetsLoaded();
}
