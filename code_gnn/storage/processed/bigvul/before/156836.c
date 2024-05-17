void Document::UpdateStyleAndLayout() {
  DCHECK(IsMainThread());

  HTMLFrameOwnerElement::PluginDisposeSuspendScope suspend_plugin_dispose;

  LocalFrameView* frame_view = View();
  DCHECK(!frame_view || !frame_view->IsInPerformLayout())
      << "View layout should not be re-entrant";

  if (HTMLFrameOwnerElement* owner = LocalOwner())
    owner->GetDocument().UpdateStyleAndLayout();

  UpdateStyleAndLayoutTree();

  if (!IsActive())
    return;

  if (frame_view && frame_view->NeedsLayout())
    frame_view->UpdateLayout();

  if (Lifecycle().GetState() < DocumentLifecycle::kLayoutClean)
    Lifecycle().AdvanceTo(DocumentLifecycle::kLayoutClean);

  if (AXObjectCache* cache = ExistingAXObjectCache())
    cache->ProcessUpdatesAfterLayout(*this);

  if (LocalFrameView* frame_view_anchored = View())
    frame_view_anchored->PerformScrollAnchoringAdjustments();
}
