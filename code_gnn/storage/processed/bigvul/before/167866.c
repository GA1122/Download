 void FrameLoader::StartNavigation(const FrameLoadRequest& passed_request,
                                   FrameLoadType frame_load_type,
                                   HistoryItem* history_item) {
  CHECK(!passed_request.GetSubstituteData().IsValid());
  CHECK(frame_load_type != kFrameLoadTypeBackForward);
  CHECK(!history_item);
  return LoadInternal(passed_request, frame_load_type, history_item,
                      true  );
}
