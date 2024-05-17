const HashSet<AtomicString>& HTMLMediaElement::GetCheckedAttributeNames()
    const {
  DEFINE_STATIC_LOCAL(HashSet<AtomicString>, attribute_set, ({"src"}));
  return attribute_set;
}
