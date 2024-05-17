void WebLocalFrameImpl::ExecuteScriptInIsolatedWorld(
    int world_id,
    const WebScriptSource& source_in) {
  DCHECK(GetFrame());
  CHECK_GT(world_id, 0);
  CHECK_LT(world_id, DOMWrapperWorld::kEmbedderWorldIdLimit);

  v8::HandleScope handle_scope(ToIsolate(GetFrame()));
  GetFrame()->GetScriptController().ExecuteScriptInIsolatedWorld(world_id,
                                                                 source_in);
}
