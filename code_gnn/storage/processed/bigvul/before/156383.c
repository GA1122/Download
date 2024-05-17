Response PageHandler::Close() {
  WebContentsImpl* web_contents = GetWebContents();
  if (!web_contents)
    return Response::Error("Not attached to a page");
  web_contents->DispatchBeforeUnload(false  );
  return Response::OK();
}
