static bool layerNeedsCompositedScrolling(const RenderLayer* layer)
{
    return layer->scrollsOverflow()
        && layer->compositor()->preferCompositingToLCDTextEnabled()
        && !layer->hasDescendantWithClipPath()
        && !layer->hasAncestorWithClipPath();
}
