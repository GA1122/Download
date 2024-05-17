void SSLErrorHandler::Dispatch() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  WebContents* web_contents = NULL;
  RenderViewHostImpl* render_view_host =
      RenderViewHostImpl::FromID(render_process_id_, render_view_id_);
  if (render_view_host)
    web_contents = render_view_host->GetDelegate()->GetAsWebContents();

  if (!web_contents) {
    OnDispatchFailed();
    return;
  }

  manager_ =
      static_cast<NavigationControllerImpl*>(&web_contents->GetController())->
          ssl_manager();
  OnDispatched();
}
