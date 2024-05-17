void PaintController::GenerateRasterInvalidationsComparingChunks(
    PaintChunk& new_chunk,
    const PaintChunk& old_chunk) {
  DCHECK(RuntimeEnabledFeatures::SlimmingPaintV175Enabled());


  struct OldAndNewDisplayItems {
    const DisplayItem* old_item = nullptr;
    const DisplayItem* new_item = nullptr;
  };
  HashMap<const DisplayItemClient*, OldAndNewDisplayItems>
      clients_to_invalidate;

  size_t highest_moved_to_index = 0;
  for (size_t old_index = old_chunk.begin_index;
       old_index < old_chunk.end_index; ++old_index) {
    const DisplayItem& old_item =
        current_paint_artifact_.GetDisplayItemList()[old_index];
    const DisplayItemClient* client_to_invalidate_old_visual_rect = nullptr;

    if (old_item.IsTombstone()) {
      size_t moved_to_index = items_moved_into_new_list_[old_index];
      if (new_display_item_list_[moved_to_index].DrawsContent()) {
        if (moved_to_index < new_chunk.begin_index ||
            moved_to_index >= new_chunk.end_index) {
          const auto& new_item = new_display_item_list_[moved_to_index];
          PaintChunk& moved_to_chunk =
              new_paint_chunks_.FindChunkByDisplayItemIndex(moved_to_index);
          AddRasterInvalidation(new_item.Client(), moved_to_chunk,
                                new_item.VisualRect(),
                                PaintInvalidationReason::kAppeared);
          client_to_invalidate_old_visual_rect = &new_item.Client();
        } else if (moved_to_index < highest_moved_to_index) {
          client_to_invalidate_old_visual_rect =
              &new_display_item_list_[moved_to_index].Client();
        } else {
          highest_moved_to_index = moved_to_index;
        }
      }
    } else if (old_item.DrawsContent()) {
      client_to_invalidate_old_visual_rect = &old_item.Client();
    }

    if (client_to_invalidate_old_visual_rect) {
      clients_to_invalidate
          .insert(client_to_invalidate_old_visual_rect, OldAndNewDisplayItems())
          .stored_value->value.old_item = &old_item;
    }
  }

  for (size_t new_index = new_chunk.begin_index;
       new_index < new_chunk.end_index; ++new_index) {
    const DisplayItem& new_item = new_display_item_list_[new_index];
    if (new_item.DrawsContent() && !ClientCacheIsValid(new_item.Client())) {
      clients_to_invalidate.insert(&new_item.Client(), OldAndNewDisplayItems())
          .stored_value->value.new_item = &new_item;
    }
  }

  for (const auto& item : clients_to_invalidate) {
    GenerateRasterInvalidation(*item.key, new_chunk, item.value.old_item,
                               item.value.new_item);
  }
}
