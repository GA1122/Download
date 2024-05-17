bool AXLayoutObject::computeAccessibilityIsIgnored(
    IgnoredReasons* ignoredReasons) const {
#if DCHECK_IS_ON()
  ASSERT(m_initialized);
#endif

  if (!m_layoutObject)
    return true;

  AXObjectInclusion decision = defaultObjectInclusion(ignoredReasons);
  if (decision == IncludeObject)
    return false;
  if (decision == IgnoreObject)
    return true;

  if (m_layoutObject->isAnonymousBlock())
    return true;

  if (isDescendantOfLeafNode()) {
    if (ignoredReasons)
      ignoredReasons->push_back(
          IgnoredReason(AXAncestorIsLeafNode, leafNodeAncestor()));
    return true;
  }

  if (roleValue() == IgnoredRole) {
    if (ignoredReasons)
      ignoredReasons->push_back(IgnoredReason(AXUninteresting));
    return true;
  }

  if (hasInheritedPresentationalRole()) {
    if (ignoredReasons) {
      const AXObject* inheritsFrom = inheritsPresentationalRoleFrom();
      if (inheritsFrom == this)
        ignoredReasons->push_back(IgnoredReason(AXPresentationalRole));
      else
        ignoredReasons->push_back(
            IgnoredReason(AXInheritsPresentation, inheritsFrom));
    }
    return true;
  }

  if (AXObject* treeAncestor = treeAncestorDisallowingChild()) {
    if (ignoredReasons)
      ignoredReasons->push_back(
          IgnoredReason(AXAncestorDisallowsChild, treeAncestor));
    return true;
  }

  if (m_layoutObject->isLayoutPart())
    return false;

  if (getLayoutObject() && getLayoutObject()->hasLayer() && getNode() &&
      getNode()->hasChildren())
    return false;

  AXObject* controlObject = correspondingControlForLabelElement();
  if (controlObject && controlObject->isCheckboxOrRadio() &&
      controlObject->nameFromLabelElement()) {
    if (ignoredReasons) {
      HTMLLabelElement* label = labelElementContainer();
      if (label && label != getNode()) {
        AXObject* labelAXObject = axObjectCache().getOrCreate(label);
        ignoredReasons->push_back(
            IgnoredReason(AXLabelContainer, labelAXObject));
      }

      ignoredReasons->push_back(IgnoredReason(AXLabelFor, controlObject));
    }
    return true;
  }

  if (m_layoutObject->isBR())
    return false;

  if (isLink())
    return false;

  if (isInPageLinkTarget())
    return false;

  if (m_layoutObject->isText()) {
    AXObject* parent = parentObjectUnignored();
    if (parent && (parent->ariaRoleAttribute() == MenuItemRole ||
                   parent->ariaRoleAttribute() == MenuButtonRole)) {
      if (ignoredReasons)
        ignoredReasons->push_back(
            IgnoredReason(AXStaticTextUsedAsNameFor, parent));
      return true;
    }
    LayoutText* layoutText = toLayoutText(m_layoutObject);
    if (!layoutText->hasTextBoxes()) {
      if (ignoredReasons)
        ignoredReasons->push_back(IgnoredReason(AXEmptyText));
      return true;
    }

    for (AXObject* parent = parentObject(); parent;
         parent = parent->parentObject()) {
      if (parent->roleValue() == TextFieldRole)
        return false;
    }

    if (layoutText->text().impl()->containsOnlyWhitespace()) {
      if (ignoredReasons)
        ignoredReasons->push_back(IgnoredReason(AXEmptyText));
      return true;
    }
    return false;
  }

  if (isHeading())
    return false;

  if (isLandmarkRelated())
    return false;

  if (getNode() &&
      (getNode()->hasTagName(headerTag) || getNode()->hasTagName(footerTag)))
    return false;

  if (isControl())
    return false;

  if (ariaRoleAttribute() != UnknownRole)
    return false;

  Node* node = m_layoutObject->node();
  if (isHTMLLabelElement(node))
    return false;

  if (hasContentEditableAttributeSet())
    return false;

  if (roleValue() == AbbrRole)
    return false;

  if (roleValue() == ListItemRole)
    return false;

  if (roleValue() == BlockquoteRole)
    return false;

  if (roleValue() == DialogRole)
    return false;

  if (roleValue() == FigcaptionRole)
    return false;

  if (roleValue() == FigureRole)
    return false;

  if (roleValue() == DetailsRole)
    return false;

  if (roleValue() == MarkRole)
    return false;

  if (roleValue() == MathRole)
    return false;

  if (roleValue() == MeterRole)
    return false;

  if (roleValue() == RubyRole)
    return false;

  if (roleValue() == SplitterRole)
    return false;

  if (roleValue() == TimeRole)
    return false;

  if (supportsARIAAttributes())
    return false;

  if (isHTMLSpanElement(node)) {
    if (ignoredReasons)
      ignoredReasons->push_back(IgnoredReason(AXUninteresting));
    return true;
  }

  if (isImage())
    return false;

  if (isCanvas()) {
    if (canvasHasFallbackContent())
      return false;
    LayoutHTMLCanvas* canvas = toLayoutHTMLCanvas(m_layoutObject);
    if (canvas->size().height() <= 1 || canvas->size().width() <= 1) {
      if (ignoredReasons)
        ignoredReasons->push_back(IgnoredReason(AXProbablyPresentational));
      return true;
    }
  }

  if (isWebArea() || m_layoutObject->isListMarker())
    return false;

  if (!getAttribute(aria_helpAttr).isEmpty() ||
      !getAttribute(aria_describedbyAttr).isEmpty() ||
      !getAttribute(altAttr).isEmpty() || !getAttribute(titleAttr).isEmpty())
    return false;

  if (isGenericFocusableElement() && node->hasChildren())
    return false;

  if (isScrollableContainer())
    return false;

  if (m_layoutObject->isLayoutBlockFlow() && m_layoutObject->childrenInline() &&
      !canSetFocusAttribute()) {
    bool hasAnyText = !!toLayoutBlockFlow(m_layoutObject)->firstLineBox();

    if (hasAnyText || mouseButtonListener())
      return false;

    if (ignoredReasons)
      ignoredReasons->push_back(IgnoredReason(AXUninteresting));
    return true;
  }

  if (ignoredReasons)
    ignoredReasons->push_back(IgnoredReason(AXUninteresting));
  return true;
}
