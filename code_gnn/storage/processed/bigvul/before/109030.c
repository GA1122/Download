void RenderViewImpl::SendUpdatedFrameTree(
    WebKit::WebFrame* exclude_frame_subtree) {
  std::string json;
  base::DictionaryValue tree;

  ConstructFrameTree(webview()->mainFrame(), exclude_frame_subtree, &tree);
  base::JSONWriter::Write(&tree, &json);

  Send(new ViewHostMsg_FrameTreeUpdated(routing_id_, json));
}
