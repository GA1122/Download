static void PrepareDataTransferForImageDrag(LocalFrame* source,
                                            DataTransfer* data_transfer,
                                            Element* node,
                                            const KURL& link_url,
                                            const KURL& image_url,
                                            const String& label) {
  node->GetDocument().UpdateStyleAndLayoutTree();
  if (HasRichlyEditableStyle(*node)) {
    Range* range = source->GetDocument()->createRange();
    range->selectNode(node, ASSERT_NO_EXCEPTION);
    source->Selection().SetSelectionAndEndTyping(
        SelectionInDOMTree::Builder()
            .SetBaseAndExtent(EphemeralRange(range))
            .Build());
  }
  data_transfer->DeclareAndWriteDragImage(node, link_url, image_url, label);
}
