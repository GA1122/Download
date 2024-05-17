int AXNodeObject::setSize() const {
  if (supportsSetSizeAndPosInSet()) {
    String setSizeStr = getAttribute(aria_setsizeAttr);
    if (!setSizeStr.isEmpty()) {
      int setSize = setSizeStr.toInt();
      if (setSize > 0)
        return setSize;
      return 1;
    }

    if (parentObject()) {
      const auto& siblings = parentObject()->children();
      return siblings.size();
    }
  }

  return 0;
}
