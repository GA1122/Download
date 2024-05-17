void Document::UpdateStyleInvalidationIfNeeded() {
  DCHECK(IsActive());
  ScriptForbiddenScope forbid_script;

  if (!ChildNeedsStyleInvalidation() && !NeedsStyleInvalidation())
    return;
  TRACE_EVENT0("blink", "Document::updateStyleInvalidationIfNeeded");
  SCOPED_BLINK_UMA_HISTOGRAM_TIMER_HIGHRES("Style.InvalidationTime");
  GetStyleEngine().InvalidateStyle();
}
