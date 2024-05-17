scoped_refptr<cc::DisplayItemList> PaintChunksToCcLayer::Convert(
    const PaintChunkSubset& paint_chunks,
    const PropertyTreeState& layer_state,
    const gfx::Vector2dF& layer_offset,
    const DisplayItemList& display_items,
    cc::DisplayItemList::UsageHint hint,
    RasterUnderInvalidationCheckingParams* under_invalidation_checking_params) {
  auto cc_list = base::MakeRefCounted<cc::DisplayItemList>(hint);
  ConvertInto(paint_chunks, layer_state, layer_offset, FloatSize(),
              display_items, *cc_list);

  if (under_invalidation_checking_params) {
    auto& params = *under_invalidation_checking_params;
    PaintRecorder recorder;
    recorder.beginRecording(params.interest_rect);
    auto list_clone = base::MakeRefCounted<cc::DisplayItemList>(
        cc::DisplayItemList::kToBeReleasedAsPaintOpBuffer);
    ConvertInto(paint_chunks, layer_state, layer_offset, FloatSize(),
                display_items, *list_clone);
    recorder.getRecordingCanvas()->drawPicture(list_clone->ReleaseAsRecord());
    params.tracking.CheckUnderInvalidations(params.debug_name,
                                            recorder.finishRecordingAsPicture(),
                                            params.interest_rect);
    if (auto record = params.tracking.UnderInvalidationRecord()) {
      cc_list->StartPaint();
      cc_list->push<cc::DrawRecordOp>(std::move(record));
      cc_list->EndPaintOfUnpaired(params.interest_rect);
    }
  }

  cc_list->Finalize();
  return cc_list;
}
