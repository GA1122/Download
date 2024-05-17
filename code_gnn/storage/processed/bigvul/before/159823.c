static bool IsSemicolonSeparatedAttribute(
    const HTMLToken::Attribute& attribute) {
  return ThreadSafeMatch(attribute.NameAsVector(), SVGNames::valuesAttr);
}
