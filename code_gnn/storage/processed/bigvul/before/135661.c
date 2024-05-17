void FrameSelection::RevealSelection(const ScrollAlignment& alignment,
                                     RevealExtentOption reveal_extent_option) {
  DCHECK(IsAvailable());

  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  const VisibleSelection& selection = ComputeVisibleSelectionInDOMTree();
  if (selection.IsNone())
    return;

  if (DocumentLoader* document_loader = frame_->Loader().GetDocumentLoader())
    document_loader->GetInitialScrollState().was_scrolled_by_user = true;
  const Position& start = selection.Start();
  DCHECK(start.AnchorNode());
  DCHECK(start.AnchorNode()->GetLayoutObject());
  GetDocument().EnsurePaintLocationDataValidForNode(start.AnchorNode());
  if (!start.AnchorNode()->GetLayoutObject()->ScrollRectToVisible(
          LayoutRect(ComputeRectToScroll(reveal_extent_option)), alignment,
          alignment))
    return;

  UpdateAppearance();
}
