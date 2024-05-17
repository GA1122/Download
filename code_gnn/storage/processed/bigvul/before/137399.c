void RenderViewTest::OnSameDocumentNavigation(blink::WebLocalFrame* frame,
                                              bool is_new_navigation,
                                              bool content_initiated) {
  RenderViewImpl* impl = static_cast<RenderViewImpl*>(view_);
  blink::WebHistoryItem item;
  item.Initialize();

  const blink::WebHistoryItem& current_item =
      impl->GetMainRenderFrame()->current_history_item();
  DCHECK(!current_item.IsNull());
  item.SetDocumentSequenceNumber(current_item.DocumentSequenceNumber());

  impl->GetMainRenderFrame()->DidNavigateWithinPage(
      item,
      is_new_navigation ? blink::kWebStandardCommit
                        : blink::kWebHistoryInertCommit,
      content_initiated);
}
