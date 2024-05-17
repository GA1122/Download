static const String UniqueSimpleSelectorAmongSiblings(Element* element) {
  DCHECK(element);

  if (element->HasID() &&
      !element->GetDocument().ContainsMultipleElementsWithId(
          element->GetIdAttribute())) {
    StringBuilder builder;
    builder.Append("#");
    SerializeIdentifier(element->GetIdAttribute(), builder);
    return builder.ToAtomicString();
  }

  if (IsOnlySiblingWithTagName(element)) {
    StringBuilder builder;
    SerializeIdentifier(element->TagQName().ToString(), builder);
    return builder.ToAtomicString();
  }

  if (element->HasClass()) {
    AtomicString unique_classname = UniqueClassnameAmongSiblings(element);
    if (!unique_classname.IsEmpty()) {
      return AtomicString(".") + unique_classname;
    }
  }

  return ":nth-child(" +
         String::Number(NthIndexCache::NthChildIndex(*element)) + ")";
}
