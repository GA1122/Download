void Document::NotifyLayoutTreeOfSubtreeChanges() {
  if (!GetLayoutViewItem().WasNotifiedOfSubtreeChange())
    return;

  lifecycle_.AdvanceTo(DocumentLifecycle::kInLayoutSubtreeChange);

  GetLayoutViewItem().HandleSubtreeModifications();
  DCHECK(!GetLayoutViewItem().WasNotifiedOfSubtreeChange());

  lifecycle_.AdvanceTo(DocumentLifecycle::kLayoutSubtreeChangeClean);
}
