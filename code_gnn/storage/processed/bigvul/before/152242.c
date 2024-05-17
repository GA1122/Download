RenderFrameImpl::CreateWorkerFetchContextForPlzDedicatedWorker(
    blink::WebDedicatedWorkerHostFactoryClient* factory_client) {
  DCHECK(blink::features::IsPlzDedicatedWorkerEnabled());
  DCHECK(factory_client);

  blink::mojom::RendererPreferenceWatcherPtr watcher;
  blink::mojom::RendererPreferenceWatcherRequest watcher_request =
      mojo::MakeRequest(&watcher);
  render_view()->RegisterRendererPreferenceWatcher(std::move(watcher));

  scoped_refptr<WebWorkerFetchContextImpl> worker_fetch_context =
      static_cast<DedicatedWorkerHostFactoryClient*>(factory_client)
          ->CreateWorkerFetchContext(render_view_->renderer_preferences(),
                                     std::move(watcher_request));

  worker_fetch_context->set_ancestor_frame_id(routing_id_);
  worker_fetch_context->set_frame_request_blocker(frame_request_blocker_);
  worker_fetch_context->set_site_for_cookies(
      frame_->GetDocument().SiteForCookies());
  worker_fetch_context->set_top_frame_origin(
      frame_->GetDocument().TopFrameOrigin());
  worker_fetch_context->set_is_secure_context(
      frame_->GetDocument().IsSecureContext());
  worker_fetch_context->set_origin_url(
      GURL(frame_->GetDocument().Url()).GetOrigin());

  for (auto& observer : observers_)
    observer.WillCreateWorkerFetchContext(worker_fetch_context.get());
  return worker_fetch_context;
}
