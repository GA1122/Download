void InspectorPageAgent::GetResourceContentAfterResourcesContentLoaded(
    const String& frame_id,
    const String& url,
    std::unique_ptr<GetResourceContentCallback> callback) {
  LocalFrame* frame =
      IdentifiersFactory::FrameById(inspected_frames_, frame_id);
  if (!frame) {
    callback->sendFailure(Response::Error("No frame for given id found"));
    return;
  }
  String content;
  bool base64_encoded;
  if (InspectorPageAgent::CachedResourceContent(
          CachedResource(frame, KURL(kParsedURLString, url),
                         inspector_resource_content_loader_),
          &content, &base64_encoded))
    callback->sendSuccess(content, base64_encoded);
  else
    callback->sendFailure(Response::Error("No resource with given URL found"));
}
