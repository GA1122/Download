void WebLocalFrameImpl::ExecuteScriptInIsolatedWorld(
    int world_id,
    const WebScriptSource* sources_in,
    unsigned num_sources,
    WebVector<v8::Local<v8::Value>>* results) {
  DCHECK(GetFrame());
  CHECK_GT(world_id, 0);
  CHECK_LT(world_id, DOMWrapperWorld::kEmbedderWorldIdLimit);

  HeapVector<ScriptSourceCode> sources =
      CreateSourcesVector(sources_in, num_sources);

  if (results) {
    Vector<v8::Local<v8::Value>> script_results;
    GetFrame()->GetScriptController().ExecuteScriptInIsolatedWorld(
        world_id, sources, &script_results);
    WebVector<v8::Local<v8::Value>> v8_results(script_results.size());
    for (unsigned i = 0; i < script_results.size(); i++) {
      v8_results[i] =
          v8::Local<v8::Value>::New(ToIsolate(GetFrame()), script_results[i]);
    }
    results->Swap(v8_results);
  } else {
    v8::HandleScope handle_scope(ToIsolate(GetFrame()));
    GetFrame()->GetScriptController().ExecuteScriptInIsolatedWorld(world_id,
                                                                   sources, 0);
  }
}
