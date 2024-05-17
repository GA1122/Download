void Document::UpdateStyleAndLayoutTree() {
  DCHECK(IsMainThread());
  if (Lifecycle().LifecyclePostponed())
    return;

  HTMLFrameOwnerElement::PluginDisposeSuspendScope suspend_plugin_dispose;
  ScriptForbiddenScope forbid_script;

  if (HTMLFrameOwnerElement* owner = LocalOwner()) {
    owner->GetDocument().UpdateStyleAndLayoutTree();
  }

  if (!View() || !IsActive())
    return;

  if (View()->ShouldThrottleRendering())
    return;

  GetSlotAssignmentEngine().RecalcSlotAssignments();

  SlotAssignmentRecalcForbiddenScope forbid_slot_recalc(*this);

  if (!NeedsLayoutTreeUpdate()) {
    if (Lifecycle().GetState() < DocumentLifecycle::kStyleClean) {
      Lifecycle().AdvanceTo(DocumentLifecycle::kInStyleRecalc);
      Lifecycle().AdvanceTo(DocumentLifecycle::kStyleClean);
    }
    return;
  }

  if (InStyleRecalc())
    return;

  CHECK(Lifecycle().StateAllowsTreeMutations());

  TRACE_EVENT_BEGIN1("blink,devtools.timeline", "UpdateLayoutTree", "beginData",
                     inspector_recalculate_styles_event::Data(GetFrame()));

  unsigned start_element_count = GetStyleEngine().StyleForElementCount();

  probe::RecalculateStyle recalculate_style_scope(this);

  DocumentAnimations::UpdateAnimationTimingIfNeeded(*this);
  EvaluateMediaQueryListIfNeeded();
  UpdateUseShadowTreesIfNeeded();

  UpdateDistributionForLegacyDistributedNodes();

  UpdateActiveStyle();
  UpdateStyleInvalidationIfNeeded();


  UpdateStyle();

  NotifyLayoutTreeOfSubtreeChanges();

  if (HoverElement() && !HoverElement()->GetLayoutObject() && GetFrame()) {
    GetFrame()->GetEventHandler().MayUpdateHoverWhenContentUnderMouseChanged(
        MouseEventManager::UpdateHoverReason::kLayoutOrStyleChanged);
  }

  if (focused_element_ && !focused_element_->IsFocusable())
    ClearFocusedElementSoon();
  GetLayoutView()->ClearHitTestCache();

  DCHECK(!DocumentAnimations::NeedsAnimationTimingUpdate(*this));

  unsigned element_count =
      GetStyleEngine().StyleForElementCount() - start_element_count;

  TRACE_EVENT_END1("blink,devtools.timeline", "UpdateLayoutTree",
                   "elementCount", element_count);

#if DCHECK_IS_ON()
  AssertLayoutTreeUpdated(*this);
#endif
}
