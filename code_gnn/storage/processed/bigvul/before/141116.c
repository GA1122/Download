void Document::ImplicitClose() {
  DCHECK(!InStyleRecalc());

  load_event_progress_ = kLoadEventInProgress;

  DetachParser();


  if (SvgExtensions())
    AccessSVGExtensions().DispatchSVGLoadEventToOutermostSVGElements();

  if (domWindow())
    domWindow()->DocumentWasClosed();

  if (GetFrame()) {
    GetFrame()->Client()->DispatchDidHandleOnloadEvents();
    Loader()->GetApplicationCacheHost()->StopDeferringEvents();
  }

  if (!GetFrame()) {
    load_event_progress_ = kLoadEventCompleted;
    return;
  }


  if (GetFrame()->GetNavigationScheduler().LocationChangePending() &&
      ElapsedTime() < kCLayoutScheduleThreshold) {
    load_event_progress_ = kLoadEventCompleted;
    return;
  }

  if (!LocalOwner() || (LocalOwner()->GetLayoutObject() &&
                        !LocalOwner()->GetLayoutObject()->NeedsLayout())) {
    UpdateStyleAndLayoutTree();

    if (View() && GetLayoutView() &&
        (!GetLayoutView()->FirstChild() || GetLayoutView()->NeedsLayout()))
      View()->UpdateLayout();
  }

  load_event_progress_ = kLoadEventCompleted;

  if (GetFrame() && GetLayoutView()) {
    if (AXObjectCache* cache = ExistingAXObjectCache()) {
      if (this == &AXObjectCacheOwner())
        cache->HandleLoadComplete(this);
      else
        cache->HandleLayoutComplete(this);
    }
  }

  if (SvgExtensions())
    AccessSVGExtensions().StartAnimations();
}
