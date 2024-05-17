CreateContextProviderOnWorkerThread(
    Platform::ContextAttributes context_attributes,
    Platform::GraphicsInfo* gl_info,
    bool* using_gpu_compositing,
    const KURL& url) {
  WaitableEvent waitable_event;
  ContextProviderCreationInfo creation_info;
  creation_info.context_attributes = context_attributes;
  creation_info.gl_info = gl_info;
  creation_info.url = url.Copy();
  creation_info.using_gpu_compositing = using_gpu_compositing;
  scoped_refptr<WebTaskRunner> task_runner =
      Platform::Current()->MainThread()->GetWebTaskRunner();
  PostCrossThreadTask(*task_runner, FROM_HERE,
                      CrossThreadBind(&CreateContextProviderOnMainThread,
                                      CrossThreadUnretained(&creation_info),
                                      CrossThreadUnretained(&waitable_event)));
  waitable_event.Wait();
  return std::move(creation_info.created_context_provider);
}
