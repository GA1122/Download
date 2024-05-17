void FrameLoader::CommitNavigation(const FrameLoadRequest& passed_request,
                                   FrameLoadType frame_load_type,
                                   HistoryItem* history_item) {
  CHECK(!passed_request.OriginDocument());
  CHECK(passed_request.FrameName().IsEmpty());
  CHECK(!passed_request.TriggeringEvent());
  CHECK(!passed_request.Form());
  return LoadInternal(passed_request, frame_load_type, history_item,
                      false  );
}
