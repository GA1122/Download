void Document::UpdateStyleAndLayout() {
  DCHECK(IsMainThread());

  ScriptForbiddenScope forbid_script;

  LocalFrameView* frame_view = View();
  if (frame_view && frame_view->IsInPerformLayout()) {
    NOTREACHED();
    return;
  }

  if (HTMLFrameOwnerElement* owner = LocalOwner())
    owner->GetDocument().UpdateStyleAndLayout();

  UpdateStyleAndLayoutTree();

  if (!IsActive())
    return;

  if (frame_view->NeedsLayout())
    frame_view->UpdateLayout();

  if (Lifecycle().GetState() < DocumentLifecycle::kLayoutClean)
    Lifecycle().AdvanceTo(DocumentLifecycle::kLayoutClean);

  if (LocalFrameView* frame_view = View())
    frame_view->PerformScrollAnchoringAdjustments();
}
