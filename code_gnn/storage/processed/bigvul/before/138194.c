String AXObject::recursiveTextAlternative(const AXObject& axObj,
                                          bool inAriaLabelledByTraversal,
                                          AXObjectSet& visited) {
  if (visited.contains(&axObj) && !inAriaLabelledByTraversal)
    return String();

  AXNameFrom tmpNameFrom;
  return axObj.textAlternative(true, inAriaLabelledByTraversal, visited,
                               tmpNameFrom, nullptr, nullptr);
 }
