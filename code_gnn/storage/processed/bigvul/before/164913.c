void ResourceDispatcherHostImpl::BeginURLRequest(
    std::unique_ptr<net::URLRequest> request,
    std::unique_ptr<ResourceHandler> handler,
    bool is_download,
    bool is_content_initiated,
    bool do_not_prompt_for_login,
    ResourceContext* context) {
  DCHECK(io_thread_task_runner_->BelongsToCurrentThread());
  DCHECK(!request->is_pending());

  ResourceRequestInfoImpl* info =
      ResourceRequestInfoImpl::ForRequest(request.get());
  DCHECK(info);
  info->set_do_not_prompt_for_login(do_not_prompt_for_login);

  if (is_download && delegate()) {
    if (request->original_url().SchemeIs(url::kBlobScheme) &&
        !storage::BlobProtocolHandler::GetRequestBlobDataHandle(
            request.get())) {
      ChromeBlobStorageContext* blob_context =
          GetChromeBlobStorageContextForResourceContext(context);
      storage::BlobProtocolHandler::SetRequestedBlobDataHandle(
          request.get(),
          blob_context->context()->GetBlobDataFromPublicURL(
              request->original_url()));
    }
    handler = HandleDownloadStarted(
        request.get(), std::move(handler), is_content_initiated,
        true  , true  );
  }
  BeginRequestInternal(std::move(request), std::move(handler),
                       false  ,
                       nullptr  );
}
