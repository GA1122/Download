void SynchronousCompositor::SetClientForWebContents(
    WebContents* contents,
    SynchronousCompositorClient* client) {
  DCHECK(contents);
  if (client) {
    g_factory.Get();   
    SynchronousCompositorImpl::CreateForWebContents(contents);
  }
  SynchronousCompositorImpl* instance =
      SynchronousCompositorImpl::FromWebContents(contents);
  DCHECK(instance);
  instance->SetClient(client);
}
