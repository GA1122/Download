void DownloadResourceHandler::OnStart(
    std::unique_ptr<download::DownloadCreateInfo> create_info,
    std::unique_ptr<ByteStreamReader> stream_reader,
    const download::DownloadUrlParameters::OnStartedCallback& callback) {
  if (create_info->result ==
          download::DOWNLOAD_INTERRUPT_REASON_USER_CANCELED &&
      create_info->is_new_download) {
    if (!callback.is_null())
      base::PostTaskWithTraits(
          FROM_HERE, {BrowserThread::UI},
          base::BindOnce(callback, nullptr, create_info->result));
    return;
  }

  const ResourceRequestInfoImpl* request_info = GetRequestInfo();
  create_info->has_user_gesture = request_info->HasUserGesture();
  create_info->transition_type = request_info->GetPageTransition();

  create_info->request_handle.reset(new DownloadRequestHandle(
      AsWeakPtr(), request_info->GetWebContentsGetterForRequest()));

  int render_process_id = -1;
  int render_frame_id = -1;
  request_info->GetAssociatedRenderFrame(&render_process_id, &render_frame_id);

  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&StartOnUIThread, std::move(create_info),
                     std::move(tab_info_), std::move(stream_reader),
                     render_process_id, render_frame_id,
                     request_info->frame_tree_node_id(), callback));
}
