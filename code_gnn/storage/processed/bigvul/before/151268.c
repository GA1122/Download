protocol::Response InspectorPageAgent::createIsolatedWorld(
    const String& frame_id,
    Maybe<String> world_name,
    Maybe<bool> grant_universal_access,
    int* execution_context_id) {
  LocalFrame* frame =
      IdentifiersFactory::FrameById(inspected_frames_, frame_id);
  if (!frame)
    return Response::Error("No frame for given id found");

  RefPtr<DOMWrapperWorld> world =
      frame->GetScriptController().CreateNewInspectorIsolatedWorld(
          world_name.fromMaybe(""));
  if (!world)
    return Response::Error("Could not create isolated world");

  if (grant_universal_access.fromMaybe(false)) {
    RefPtr<SecurityOrigin> security_origin =
        frame->GetSecurityContext()->GetSecurityOrigin()->IsolatedCopy();
    security_origin->GrantUniversalAccess();
    DOMWrapperWorld::SetIsolatedWorldSecurityOrigin(world->GetWorldId(),
                                                    security_origin);
  }

  LocalWindowProxy* isolated_world_window_proxy =
      frame->GetScriptController().WindowProxy(*world);
  v8::HandleScope handle_scope(V8PerIsolateData::MainThreadIsolate());
  *execution_context_id = v8_inspector::V8ContextInfo::executionContextId(
      isolated_world_window_proxy->ContextIfInitialized());
  return Response::OK();
}
