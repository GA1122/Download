bool DownloadManagerImpl::InterceptDownload(
    const download::DownloadCreateInfo& info) {
  WebContents* web_contents = WebContentsImpl::FromRenderFrameHostID(
      info.render_process_id, info.render_frame_id);
  if (info.is_new_download &&
      info.result ==
          download::DOWNLOAD_INTERRUPT_REASON_SERVER_CROSS_ORIGIN_REDIRECT) {
    if (web_contents) {
      std::vector<GURL> url_chain(info.url_chain);
      GURL url = url_chain.back();
      url_chain.pop_back();
      NavigationController::LoadURLParams params(url);
      params.has_user_gesture = info.has_user_gesture;
      params.referrer = Referrer(
          info.referrer_url, Referrer::NetReferrerPolicyToBlinkReferrerPolicy(
                                 info.referrer_policy));
      params.redirect_chain = url_chain;
      params.frame_tree_node_id =
          RenderFrameHost::GetFrameTreeNodeIdForRoutingId(
              info.render_process_id, info.render_frame_id);
      web_contents->GetController().LoadURLWithParams(params);
    }
    if (info.request_handle)
      info.request_handle->CancelRequest(false);
    return true;
  }

  std::string user_agent = "";
  for (const auto& header : info.request_headers) {
    if (header.first == net::HttpRequestHeaders::kUserAgent) {
      user_agent = header.second;
      break;
    }
  }

  if (delegate_ &&
      delegate_->InterceptDownloadIfApplicable(
          info.url(), user_agent, info.content_disposition, info.mime_type,
          info.request_origin, info.total_bytes, web_contents)) {
    if (info.request_handle)
      info.request_handle->CancelRequest(false);
    return true;
  }
  content::devtools_instrumentation::WillBeginDownload(
      info.render_process_id, info.render_frame_id, info.url());
  return false;
}
