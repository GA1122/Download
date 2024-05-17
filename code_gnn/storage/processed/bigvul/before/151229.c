InspectorPageAgent* InspectorPageAgent::Create(
    InspectedFrames* inspected_frames,
    Client* client,
    InspectorResourceContentLoader* resource_content_loader,
    v8_inspector::V8InspectorSession* v8_session) {
  return new InspectorPageAgent(inspected_frames, client,
                                resource_content_loader, v8_session);
}
