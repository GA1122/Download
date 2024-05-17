void PaintController::GenerateRasterInvalidation(
    const DisplayItemClient& client,
    PaintChunk& chunk,
    const DisplayItem* old_item,
    const DisplayItem* new_item) {
  if (!new_item || new_item->VisualRect().IsEmpty()) {
    if (old_item && !old_item->VisualRect().IsEmpty()) {
      AddRasterInvalidation(client, chunk, old_item->VisualRect(),
                            PaintInvalidationReason::kDisappeared);
    }
    return;
  }

  DCHECK(&client == &new_item->Client());
  if (!old_item || old_item->VisualRect().IsEmpty()) {
    AddRasterInvalidation(client, chunk, new_item->VisualRect(),
                          PaintInvalidationReason::kAppeared);
    return;
  }

  if (client.IsJustCreated()) {
    AddRasterInvalidation(client, chunk, old_item->VisualRect(),
                          PaintInvalidationReason::kDisappeared);
    AddRasterInvalidation(client, chunk, new_item->VisualRect(),
                          PaintInvalidationReason::kAppeared);
    return;
  }

  auto reason = client.GetPaintInvalidationReason();
  bool partial_raster_invalidation =
      RuntimeEnabledFeatures::PartialRasterInvalidationEnabled() &&
      (reason == PaintInvalidationReason::kRectangle ||
       reason == PaintInvalidationReason::kSelection);
  if ((!partial_raster_invalidation &&
       reason != PaintInvalidationReason::kIncremental) ||
      old_item->VisualRect().Location() != new_item->VisualRect().Location()) {
    GenerateFullRasterInvalidation(chunk, *old_item, *new_item);
    return;
  }

  GenerateIncrementalRasterInvalidation(chunk, *old_item, *new_item);

  if (RuntimeEnabledFeatures::PartialRasterInvalidationEnabled()) {
    auto partial_rect = client.PartialInvalidationRect();
    if (!partial_rect.IsEmpty())
      AddRasterInvalidation(client, chunk, FloatRect(partial_rect), reason);
  }
}
