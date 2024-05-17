String AXObject::name(NameSources* nameSources) const {
  AXObjectSet visited;
  AXNameFrom tmpNameFrom;
  AXRelatedObjectVector tmpRelatedObjects;
  String text = textAlternative(false, false, visited, tmpNameFrom,
                                &tmpRelatedObjects, nameSources);
  text = text.simplifyWhiteSpace(isHTMLSpace<UChar>);
  return text;
}
