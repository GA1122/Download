void WebViewPlugin::startDragging(const WebDragData&,
                                  WebDragOperationsMask,
                                  const WebImage&,
                                  const WebPoint&) {
  web_view_->dragSourceSystemDragEnded();
}
