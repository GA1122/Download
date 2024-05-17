Response PageHandler::Crash() {
  WebContentsImpl* web_contents = GetWebContents();
  if (!web_contents)
    return Response::Error("Not attached to a page");
  if (web_contents->IsCrashed())
    return Response::Error("The target has already crashed");
  if (web_contents->GetMainFrame()->frame_tree_node()->navigation_request())
    return Response::Error("Page has pending navigations, not killing");
  return Response::FallThrough();
}
