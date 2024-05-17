void PaintLayerScrollableArea::InvalidatePaintForScrollOffsetChange() {
  InvalidatePaintForStickyDescendants();

  auto* box = GetLayoutBox();
  auto* frame_view = box->GetFrameView();
  frame_view->InvalidateBackgroundAttachmentFixedDescendantsOnScroll(*box);

  if (box->IsLayoutView() && frame_view->HasViewportConstrainedObjects() &&
      !frame_view->InvalidateViewportConstrainedObjects()) {
    box->SetShouldDoFullPaintInvalidation();
    box->SetSubtreeShouldCheckForPaintInvalidation();
  }

  if (Layer()->EnclosingPaginationLayer())
    box->SetSubtreeShouldCheckForPaintInvalidation();

  bool background_paint_in_graphics_layer = true;
  bool background_paint_in_scrolling_contents = false;
  if (RuntimeEnabledFeatures::CompositeAfterPaintEnabled() ||
      UsesCompositedScrolling()) {
    auto background_paint_location = box->GetBackgroundPaintLocation();
    background_paint_in_graphics_layer =
        background_paint_location & kBackgroundPaintInGraphicsLayer;
    background_paint_in_scrolling_contents =
        background_paint_location & kBackgroundPaintInScrollingContents;
  }

  auto background_layers = box->StyleRef().BackgroundLayers();
  if ((background_layers.AnyLayerHasLocalAttachmentImage() &&
       background_paint_in_graphics_layer) ||
      (background_layers.AnyLayerHasDefaultAttachmentImage() &&
       background_paint_in_scrolling_contents))
    box->SetBackgroundNeedsFullPaintInvalidation();

  if (RuntimeEnabledFeatures::CompositeAfterPaintEnabled() ||
      !UsesCompositedScrolling())
    Layer()->SetNeedsRepaint();
}
