void ChromeClientImpl::OnMouseDown(Node& mouse_down_node) {
  if (auto* fill_client =
          AutofillClientFromFrame(mouse_down_node.GetDocument().GetFrame())) {
    fill_client->DidReceiveLeftMouseDownOrGestureTapInNode(
        WebNode(&mouse_down_node));
  }
}
