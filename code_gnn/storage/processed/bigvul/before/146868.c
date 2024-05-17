void Document::UpdateStyleInvalidationIfNeeded() {
  DCHECK(IsActive());
  ScriptForbiddenScope forbid_script;

  if (!ChildNeedsStyleInvalidation() && !NeedsStyleInvalidation())
    return;
  TRACE_EVENT0("blink", "Document::updateStyleInvalidationIfNeeded");
  GetStyleEngine().GetStyleInvalidator().Invalidate(*this);
}
