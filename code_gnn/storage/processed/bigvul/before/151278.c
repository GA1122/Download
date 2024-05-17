void InspectorPageAgent::searchInResource(
    const String& frame_id,
    const String& url,
    const String& query,
    Maybe<bool> optional_case_sensitive,
    Maybe<bool> optional_is_regex,
    std::unique_ptr<SearchInResourceCallback> callback) {
  if (!enabled_) {
    callback->sendFailure(Response::Error("Agent is not enabled."));
    return;
  }
  inspector_resource_content_loader_->EnsureResourcesContentLoaded(
      resource_content_loader_client_id_,
      WTF::Bind(&InspectorPageAgent::SearchContentAfterResourcesContentLoaded,
                WrapPersistent(this), frame_id, url, query,
                optional_case_sensitive.fromMaybe(false),
                optional_is_regex.fromMaybe(false),
                WTF::Passed(std::move(callback))));
}
