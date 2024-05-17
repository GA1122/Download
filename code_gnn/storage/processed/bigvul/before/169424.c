void TryCreateDuplicateRequestIds(Shell* shell, bool block_loaders) {
  NavigateToURL(shell, GURL("http://foo.com/simple_page.html"));
  RenderFrameHost* rfh = shell->web_contents()->GetMainFrame();

  if (block_loaders) {
    rfh->BlockRequestsForFrame();
  }

  const char* blocking_url = net::URLRequestSlowDownloadJob::kUnknownSizeUrl;
  ResourceRequest request(CreateXHRRequest(blocking_url));

  RenderProcessHostWatcher process_killed(
      rfh->GetProcess(), RenderProcessHostWatcher::WATCH_FOR_PROCESS_EXIT);
  IPC::IpcSecurityTestUtil::PwnMessageReceived(
      rfh->GetProcess()->GetChannel(),
      ResourceHostMsg_RequestResource(rfh->GetRoutingID(),
                                      kRequestIdNotPreviouslyUsed, request,
                                      net::MutableNetworkTrafficAnnotationTag(
                                          TRAFFIC_ANNOTATION_FOR_TESTS)));
  IPC::IpcSecurityTestUtil::PwnMessageReceived(
      rfh->GetProcess()->GetChannel(),
      ResourceHostMsg_RequestResource(rfh->GetRoutingID(),
                                      kRequestIdNotPreviouslyUsed, request,
                                      net::MutableNetworkTrafficAnnotationTag(
                                          TRAFFIC_ANNOTATION_FOR_TESTS)));
  process_killed.Wait();
}
