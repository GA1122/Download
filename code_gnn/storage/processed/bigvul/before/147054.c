void WebLocalFrameImpl::RequestExecuteScriptInIsolatedWorld(
    int world_id,
    const WebScriptSource* sources_in,
    unsigned num_sources,
    bool user_gesture,
    ScriptExecutionType option,
    WebScriptExecutionCallback* callback) {
  DCHECK(GetFrame());
  CHECK_GT(world_id, 0);
  CHECK_LT(world_id, DOMWrapperWorld::kEmbedderWorldIdLimit);

  RefPtr<DOMWrapperWorld> isolated_world =
      DOMWrapperWorld::EnsureIsolatedWorld(ToIsolate(GetFrame()), world_id);
  SuspendableScriptExecutor* executor = SuspendableScriptExecutor::Create(
      GetFrame(), std::move(isolated_world),
      CreateSourcesVector(sources_in, num_sources), user_gesture, callback);
  switch (option) {
    case kAsynchronousBlockingOnload:
      executor->RunAsync(SuspendableScriptExecutor::kOnloadBlocking);
      break;
    case kAsynchronous:
      executor->RunAsync(SuspendableScriptExecutor::kNonBlocking);
      break;
    case kSynchronous:
      executor->Run();
      break;
  }
}
