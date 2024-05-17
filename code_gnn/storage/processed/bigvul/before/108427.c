void ChromeRenderMessageFilter::OnExtensionResumeRequests(int route_id) {
  content::ResourceDispatcherHost::Get()->ResumeBlockedRequestsForRoute(
       render_process_id_, route_id);
 }
