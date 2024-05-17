static void StartOnUIThread(
    std::unique_ptr<download::DownloadCreateInfo> info,
    std::unique_ptr<DownloadResourceHandler::DownloadTabInfo> tab_info,
    std::unique_ptr<ByteStreamReader> stream,
    int render_process_id,
    int render_frame_id,
    int frame_tree_node_id,
    const download::DownloadUrlParameters::OnStartedCallback& started_cb) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  RenderFrameHost* frame_host =
      RenderFrameHost::FromID(render_process_id, render_frame_id);

  if (!frame_host) {
    FrameTreeNode* frame_tree_node =
        FrameTreeNode::GloballyFindByID(frame_tree_node_id);
    if (frame_tree_node)
      frame_host = frame_tree_node->current_frame_host();
  }

  DownloadManager* download_manager = nullptr;
  if (frame_host) {
    download_manager = BrowserContext::GetDownloadManager(
        frame_host->GetProcess()->GetBrowserContext());
  }

  if (!download_manager || !frame_host) {
    if (!started_cb.is_null())
      started_cb.Run(nullptr,
                     download::DOWNLOAD_INTERRUPT_REASON_USER_CANCELED);

    if (stream)
      download::GetDownloadTaskRunner()->DeleteSoon(FROM_HERE,
                                                    stream.release());
    return;
  }

  info->tab_url = tab_info->tab_url;
  info->tab_referrer_url = tab_info->tab_referrer_url;
  info->ukm_source_id = tab_info->ukm_source_id;
  info->site_url = frame_host->GetSiteInstance()->GetSiteURL();
  info->render_process_id = frame_host->GetProcess()->GetID();
  info->render_frame_id = frame_host->GetRoutingID();

  download_manager->StartDownload(
      std::move(info),
      std::make_unique<ByteStreamInputStream>(std::move(stream)), nullptr,
      started_cb);
}
