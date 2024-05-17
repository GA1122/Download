static bool IsOnlySiblingWithTagName(Element* element) {
  DCHECK(element);
  return (1U == NthIndexCache::NthOfTypeIndex(*element)) &&
         (1U == NthIndexCache::NthLastOfTypeIndex(*element));
}
