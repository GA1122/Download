int AXNodeObject::headingLevel() const {
  Node* node = this->getNode();
  if (!node)
    return 0;

  if (roleValue() == HeadingRole) {
    String levelStr = getAttribute(aria_levelAttr);
    if (!levelStr.isEmpty()) {
      int level = levelStr.toInt();
      if (level >= 1 && level <= 9)
        return level;
      return 1;
    }
  }

  if (!node->isHTMLElement())
    return 0;

  HTMLElement& element = toHTMLElement(*node);
  if (element.hasTagName(h1Tag))
    return 1;

  if (element.hasTagName(h2Tag))
    return 2;

  if (element.hasTagName(h3Tag))
    return 3;

  if (element.hasTagName(h4Tag))
    return 4;

  if (element.hasTagName(h5Tag))
    return 5;

  if (element.hasTagName(h6Tag))
    return 6;

  return 0;
}
