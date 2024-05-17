void WebLocalFrameImpl::RequestExecuteScriptAndReturnValue(
    const WebScriptSource& source,
    bool user_gesture,
    WebScriptExecutionCallback* callback) {
  DCHECK(GetFrame());

  scoped_refptr<DOMWrapperWorld> main_world = &DOMWrapperWorld::MainWorld();
  PausableScriptExecutor* executor = PausableScriptExecutor::Create(
      GetFrame(), std::move(main_world), CreateSourcesVector(&source, 1),
      user_gesture, callback);
  executor->Run();
}
