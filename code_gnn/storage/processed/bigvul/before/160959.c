void ChromeClientImpl::FocusedNodeChanged(Node* from_node, Node* to_node) {
  if (!web_view_->Client())
    return;

  web_view_->Client()->FocusedNodeChanged(WebNode(from_node), WebNode(to_node));

  WebURL focus_url;
  if (to_node && to_node->IsElementNode() && ToElement(to_node)->IsLiveLink() &&
      to_node->ShouldHaveFocusAppearance())
    focus_url = ToElement(to_node)->HrefURL();
  web_view_->Client()->SetKeyboardFocusURL(focus_url);
}
