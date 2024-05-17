bool Document::HaveScriptBlockingStylesheetsLoaded() const {
  return style_engine_->HaveScriptBlockingStylesheetsLoaded();
}
