void DevToolsClient::saveAs(const WebKit::WebString& file_name,
                            const WebKit::WebString& content) {
  Send(new DevToolsHostMsg_SaveAs(routing_id(),
                                  file_name.utf8(),
                                  content.utf8()));
}
