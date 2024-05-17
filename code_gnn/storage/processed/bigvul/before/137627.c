void DownloadController::StartContextMenuDownload(
    const ContextMenuParams& params, WebContents* web_contents, bool is_link,
    const std::string& extra_headers) {
  int process_id = web_contents->GetRenderProcessHost()->GetID();
  int routing_id = web_contents->GetRoutingID();
  AcquireFileAccessPermission(
      web_contents, base::Bind(&CreateContextMenuDownload, process_id,
                               routing_id, params, is_link, extra_headers));
}
