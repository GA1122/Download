void Document::NotifyLayoutTreeOfSubtreeChanges() {
  if (!GetLayoutView()->WasNotifiedOfSubtreeChange())
    return;

  lifecycle_.AdvanceTo(DocumentLifecycle::kInLayoutSubtreeChange);

  GetLayoutView()->HandleSubtreeModifications();
  DCHECK(!GetLayoutView()->WasNotifiedOfSubtreeChange());

  lifecycle_.AdvanceTo(DocumentLifecycle::kLayoutSubtreeChangeClean);
}
