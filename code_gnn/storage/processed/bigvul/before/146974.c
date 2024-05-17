void WebLocalFrameImpl::ExecuteScriptInIsolatedWorld(
    int world_id,
    const WebScriptSource* sources_in,
    unsigned num_sources) {
  DCHECK(GetFrame());
  CHECK_GT(world_id, 0);
  CHECK_LT(world_id, DOMWrapperWorld::kEmbedderWorldIdLimit);

  HeapVector<ScriptSourceCode> sources =
      CreateSourcesVector(sources_in, num_sources);
  v8::HandleScope handle_scope(ToIsolate(GetFrame()));
  GetFrame()->GetScriptController().ExecuteScriptInIsolatedWorld(world_id,
                                                                 sources, 0);
}
