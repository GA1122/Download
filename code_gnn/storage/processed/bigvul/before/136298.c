static bool NeedsEffect(const LayoutObject& object) {
  const ComputedStyle& style = object.StyleRef();

  const bool is_css_isolated_group =
      object.IsBoxModelObject() && style.IsStackingContext();

  if (!is_css_isolated_group && !object.IsSVG())
    return false;

  if (object.IsSVG()) {
    if (object.IsSVGRoot() && is_css_isolated_group &&
        object.HasNonIsolatedBlendingDescendants())
      return true;
    if (SVGLayoutSupport::IsIsolationRequired(&object))
      return true;
    if (SVGResources* resources =
            SVGResourcesCache::CachedResourcesForLayoutObject(object)) {
      if (resources->Masker()) {
        return true;
      }
    }
  } else if (object.IsBoxModelObject()) {
    if (PaintLayer* layer = ToLayoutBoxModelObject(object).Layer()) {
      if (layer->HasNonIsolatedDescendantWithBlendMode())
        return true;
    }
  }

  SkBlendMode blend_mode = object.IsBlendingAllowed()
                               ? WebCoreCompositeToSkiaComposite(
                                     kCompositeSourceOver, style.GetBlendMode())
                               : SkBlendMode::kSrcOver;
  if (blend_mode != SkBlendMode::kSrcOver)
    return true;

  float opacity = style.Opacity();
  if (opacity != 1.0f)
    return true;

  if (CompositingReasonFinder::RequiresCompositingForOpacityAnimation(style))
    return true;

  if (object.StyleRef().HasMask())
    return true;

  if (object.HasLayer() &&
      ToLayoutBoxModelObject(object).Layer()->GetCompositedLayerMapping() &&
      ToLayoutBoxModelObject(object)
          .Layer()
          ->GetCompositedLayerMapping()
          ->MaskLayer()) {
    return true;
  }

  if (object.StyleRef().ClipPath() &&
      object.FirstFragment().ClipPathBoundingBox() &&
      !object.FirstFragment().ClipPathPath()) {
    return true;
  }

  return false;
}
