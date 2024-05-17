bool PaintLayerScrollableArea::ComputeNeedsCompositedScrolling(
    const bool layer_has_been_composited,
    const PaintLayer* layer) {
  non_composited_main_thread_scrolling_reasons_ = 0;

  if (CompositingReasonFinder::RequiresCompositingForRootScroller(*layer))
    return true;

  Node* node = layer->GetLayoutObject().GetNode();
  if (!layer->ScrollsOverflow() &&
      !ScrollTimeline::HasActiveScrollTimeline(node)) {
    return false;
  }

  if (layer->Size().IsEmpty())
    return false;

  if (!layer_has_been_composited &&
      !LayerNodeMayNeedCompositedScrolling(layer)) {
    return false;
  }

  bool needs_composited_scrolling = true;

  bool background_supports_lcd_text =
      GetLayoutBox()->StyleRef().IsStackingContext() &&
      GetLayoutBox()->GetBackgroundPaintLocation(
          &non_composited_main_thread_scrolling_reasons_) &
          kBackgroundPaintInScrollingContents &&
      layer->BackgroundIsKnownToBeOpaqueInRect(
          ToLayoutBox(layer->GetLayoutObject()).PhysicalPaddingBoxRect(),
          true) &&
      !layer->CompositesWithTransform() && !layer->CompositesWithOpacity();

  if (!ScrollTimeline::HasActiveScrollTimeline(node) &&
      !layer_has_been_composited &&
      !layer->Compositor()->PreferCompositingToLCDTextEnabled() &&
      !background_supports_lcd_text) {
    if (layer->CompositesWithOpacity()) {
      non_composited_main_thread_scrolling_reasons_ |=
          MainThreadScrollingReason::kHasOpacityAndLCDText;
    }
    if (layer->CompositesWithTransform()) {
      non_composited_main_thread_scrolling_reasons_ |=
          MainThreadScrollingReason::kHasTransformAndLCDText;
    }
    if (!layer->BackgroundIsKnownToBeOpaqueInRect(
            ToLayoutBox(layer->GetLayoutObject()).PhysicalPaddingBoxRect(),
            true)) {
      non_composited_main_thread_scrolling_reasons_ |=
          MainThreadScrollingReason::kBackgroundNotOpaqueInRectAndLCDText;
    }
    if (!layer->GetLayoutObject().StyleRef().IsStackingContext()) {
      non_composited_main_thread_scrolling_reasons_ |=
          MainThreadScrollingReason::kIsNotStackingContextAndLCDText;
    }

    needs_composited_scrolling = false;
  }

  if (layer->GetLayoutObject().HasClip() ||
      layer->HasDescendantWithClipPath() || !!layer->ClipPathAncestor()) {
    non_composited_main_thread_scrolling_reasons_ |=
        MainThreadScrollingReason::kHasClipRelatedProperty;
    needs_composited_scrolling = false;
  }

  DCHECK(!(non_composited_main_thread_scrolling_reasons_ &
           ~MainThreadScrollingReason::kNonCompositedReasons));
  return needs_composited_scrolling;
}
