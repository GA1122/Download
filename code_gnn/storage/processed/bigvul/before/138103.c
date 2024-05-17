int AXNodeObject::posInSet() const {
  if (supportsSetSizeAndPosInSet()) {
    String posInSetStr = getAttribute(aria_posinsetAttr);
    if (!posInSetStr.isEmpty()) {
      int posInSet = posInSetStr.toInt();
      if (posInSet > 0)
        return posInSet;
      return 1;
    }

    return AXObject::indexInParent() + 1;
  }

  return 0;
}
