void ExtensionsGuestViewMessageFilter::CreateEmbeddedMimeHandlerViewGuest(
    int32_t render_frame_id,
    int32_t tab_id,
    const GURL& original_url,
    int32_t element_instance_id,
    const gfx::Size& element_size,
    content::mojom::TransferrableURLLoaderPtr transferrable_url_loader,
    int32_t plugin_frame_routing_id) {
  if (!content::BrowserThread::CurrentlyOn(content::BrowserThread::UI)) {
    base::PostTaskWithTraits(
        FROM_HERE, {content::BrowserThread::UI},
        base::BindOnce(&ExtensionsGuestViewMessageFilter::
                           CreateEmbeddedMimeHandlerViewGuest,
                       this, render_frame_id, tab_id, original_url,
                       element_instance_id, element_size,
                       base::Passed(&transferrable_url_loader),
                       plugin_frame_routing_id));
    return;
  }

  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(
          RenderFrameHost::FromID(render_process_id_, render_frame_id));
  if (!web_contents)
    return;

  auto* browser_context = web_contents->GetBrowserContext();
  std::string extension_id = transferrable_url_loader->url.host();
  const Extension* extension = ExtensionRegistry::Get(browser_context)
                                   ->enabled_extensions()
                                   .GetByID(extension_id);
  if (!extension)
    return;

  MimeTypesHandler* handler = MimeTypesHandler::GetHandler(extension);
  if (!handler || !handler->HasPlugin()) {
    NOTREACHED();
    return;
  }

  GURL handler_url(Extension::GetBaseURLFromExtensionId(extension_id).spec() +
                   handler->handler_url());

  std::string view_id = base::GenerateGUID();
  std::unique_ptr<StreamContainer> stream_container(new StreamContainer(
      nullptr, tab_id, true  , handler_url, extension_id,
      std::move(transferrable_url_loader), original_url));
  MimeHandlerStreamManager::Get(browser_context)
      ->AddStream(view_id, std::move(stream_container),
                  -1  , render_process_id_,
                  render_frame_id);

  CreateMimeHandlerViewGuestOnUIThread(render_frame_id, view_id,
                                       element_instance_id, element_size,
                                       nullptr, plugin_frame_routing_id, false);
}
