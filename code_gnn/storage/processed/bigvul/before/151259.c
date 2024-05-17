void InspectorPageAgent::SearchContentAfterResourcesContentLoaded(
    const String& frame_id,
    const String& url,
    const String& query,
    bool case_sensitive,
    bool is_regex,
    std::unique_ptr<SearchInResourceCallback> callback) {
  LocalFrame* frame =
      IdentifiersFactory::FrameById(inspected_frames_, frame_id);
  if (!frame) {
    callback->sendFailure(Response::Error("No frame for given id found"));
    return;
  }
  String content;
  bool base64_encoded;
  if (!InspectorPageAgent::CachedResourceContent(
          CachedResource(frame, KURL(kParsedURLString, url),
                         inspector_resource_content_loader_),
          &content, &base64_encoded)) {
    callback->sendFailure(Response::Error("No resource with given URL found"));
    return;
  }

  auto matches = v8_session_->searchInTextByLines(
      ToV8InspectorStringView(content), ToV8InspectorStringView(query),
      case_sensitive, is_regex);
  auto results = protocol::Array<
      v8_inspector::protocol::Debugger::API::SearchMatch>::create();
  for (size_t i = 0; i < matches.size(); ++i)
    results->addItem(std::move(matches[i]));
  callback->sendSuccess(std::move(results));
}
