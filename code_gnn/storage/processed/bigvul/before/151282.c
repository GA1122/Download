Response InspectorPageAgent::startScreencast(Maybe<String> format,
                                             Maybe<int> quality,
                                             Maybe<int> max_width,
                                             Maybe<int> max_height,
                                             Maybe<int> every_nth_frame) {
  state_->setBoolean(PageAgentState::kScreencastEnabled, true);
  return Response::OK();
}
