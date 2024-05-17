void WebPluginDelegatePepper::RenderViewInitiatedPaint() {
  Graphics2DMap::iterator iter2d(&graphic2d_contexts_);
  while (!iter2d.IsAtEnd()) {
    iter2d.GetCurrentValue()->RenderViewInitiatedPaint();
    iter2d.Advance();
  }
}
