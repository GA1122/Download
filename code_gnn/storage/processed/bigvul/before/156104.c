const HashSet<AtomicString>& HTMLLinkElement::GetCheckedAttributeNames() const {
  DEFINE_STATIC_LOCAL(HashSet<AtomicString>, attribute_set, ({"href"}));
  return attribute_set;
}
