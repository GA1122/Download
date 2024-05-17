void Document::UpdateStyleAndLayoutTree() {
  DCHECK(IsMainThread());

  HTMLFrameOwnerElement::PluginDisposeSuspendScope suspend_plugin_dispose;
  ScriptForbiddenScope forbid_script;

  if (!View() || !IsActive())
    return;

  if (View()->ShouldThrottleRendering())
    return;

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
                     InspectorRecalculateStylesEvent::Data(GetFrame()));

  unsigned start_element_count = GetStyleEngine().StyleForElementCount();

  probe::RecalculateStyle recalculate_style_scope(this);

  DocumentAnimations::UpdateAnimationTimingIfNeeded(*this);
  EvaluateMediaQueryListIfNeeded();
  UpdateUseShadowTreesIfNeeded();
  UpdateDistribution();
  UpdateActiveStyle();
  UpdateStyleInvalidationIfNeeded();


  UpdateStyle();

  NotifyLayoutTreeOfSubtreeChanges();

  if (HoverElement() && !HoverElement()->GetLayoutObject() && GetFrame()) {
    GetFrame()->GetEventHandler().DispatchFakeMouseMoveEventSoon(
        MouseEventManager::FakeMouseMoveReason::kPerFrame);
  }

  if (focused_element_ && !focused_element_->IsFocusable())
    ClearFocusedElementSoon();
  GetLayoutViewItem().ClearHitTestCache();

  DCHECK(!DocumentAnimations::NeedsAnimationTimingUpdate(*this));

  unsigned element_count =
      GetStyleEngine().StyleForElementCount() - start_element_count;

  TRACE_EVENT_END1("blink,devtools.timeline", "UpdateLayoutTree",
                   "elementCount", element_count);

#if DCHECK_IS_ON()
  AssertLayoutTreeUpdated(*this);
#endif
}
