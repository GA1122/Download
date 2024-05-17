void WebPluginDelegatePepper::RenderViewFlushedPaint() {
  Graphics2DMap::iterator iter2d(&graphic2d_contexts_);
  while (!iter2d.IsAtEnd()) {
    iter2d.GetCurrentValue()->RenderViewFlushedPaint();
    iter2d.Advance();
  }
}
