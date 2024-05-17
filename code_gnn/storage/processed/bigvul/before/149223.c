void HTMLFormControlElement::DisabledAttributeChanged() {
  EventDispatchForbiddenScope event_forbidden;

  ListedElement::DisabledAttributeChanged();
  if (LayoutObject* o = GetLayoutObject())
    o->InvalidateIfControlStateChanged(kEnabledControlState);

  if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
    cache->CheckedStateChanged(this);
}
