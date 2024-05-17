void SynchronousCompositor::SetClientForWebContents(
    WebContents* contents,
    SynchronousCompositorClient* client) {
  DCHECK(contents);
  DCHECK(client);
  g_factory.Get();   
  WebContentsAndroid* web_contents_android =
      static_cast<WebContentsImpl*>(contents)->GetWebContentsAndroid();
  DCHECK(!web_contents_android->synchronous_compositor_client());
  web_contents_android->set_synchronous_compositor_client(client);
}
