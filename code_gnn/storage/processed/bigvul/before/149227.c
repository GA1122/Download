int32_t HTMLFormControlElement::GetAxId() const {
  if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
    return cache->GetAXID(const_cast<HTMLFormControlElement*>(this));

  return 0;
}
