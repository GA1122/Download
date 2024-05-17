static bool HasChildTags(Element& element, const QualifiedName& tag_name) {
  return !element.getElementsByTagName(tag_name.LocalName())->IsEmpty();
}
