AccessibilityRole AXNodeObject::nativeAccessibilityRoleIgnoringAria() const {
  if (!getNode())
    return UnknownRole;

  if (getNode()->isLink())
    return LinkRole;

  if (isHTMLAnchorElement(*getNode())) {
    if (isClickable())
      return LinkRole;
    return AnchorRole;
  }

  if (isHTMLButtonElement(*getNode()))
    return buttonRoleType();

  if (isHTMLDetailsElement(*getNode()))
    return DetailsRole;

  if (isHTMLSummaryElement(*getNode())) {
    ContainerNode* parent = FlatTreeTraversal::parent(*getNode());
    if (parent && isHTMLDetailsElement(parent))
      return DisclosureTriangleRole;
    return UnknownRole;
  }

  if (isHTMLInputElement(*getNode())) {
    HTMLInputElement& input = toHTMLInputElement(*getNode());
    const AtomicString& type = input.type();
    if (input.dataList())
      return ComboBoxRole;
    if (type == InputTypeNames::button) {
      if ((getNode()->parentNode() &&
           isHTMLMenuElement(getNode()->parentNode())) ||
          (parentObject() && parentObject()->roleValue() == MenuRole))
        return MenuItemRole;
      return buttonRoleType();
    }
    if (type == InputTypeNames::checkbox) {
      if ((getNode()->parentNode() &&
           isHTMLMenuElement(getNode()->parentNode())) ||
          (parentObject() && parentObject()->roleValue() == MenuRole))
        return MenuItemCheckBoxRole;
      return CheckBoxRole;
    }
    if (type == InputTypeNames::date)
      return DateRole;
    if (type == InputTypeNames::datetime ||
        type == InputTypeNames::datetime_local ||
        type == InputTypeNames::month || type == InputTypeNames::week)
      return DateTimeRole;
    if (type == InputTypeNames::file)
      return ButtonRole;
    if (type == InputTypeNames::radio) {
      if ((getNode()->parentNode() &&
           isHTMLMenuElement(getNode()->parentNode())) ||
          (parentObject() && parentObject()->roleValue() == MenuRole))
        return MenuItemRadioRole;
      return RadioButtonRole;
    }
    if (type == InputTypeNames::number)
      return SpinButtonRole;
    if (input.isTextButton())
      return buttonRoleType();
    if (type == InputTypeNames::range)
      return SliderRole;
    if (type == InputTypeNames::color)
      return ColorWellRole;
    if (type == InputTypeNames::time)
      return InputTimeRole;
    return TextFieldRole;
  }

  if (isHTMLSelectElement(*getNode())) {
    HTMLSelectElement& selectElement = toHTMLSelectElement(*getNode());
    return selectElement.isMultiple() ? ListBoxRole : PopUpButtonRole;
  }

  if (isHTMLTextAreaElement(*getNode()))
    return TextFieldRole;

  if (headingLevel())
    return HeadingRole;

  if (isHTMLDivElement(*getNode()))
    return DivRole;

  if (isHTMLMeterElement(*getNode()))
    return MeterRole;

  if (isHTMLOutputElement(*getNode()))
    return StatusRole;

  if (isHTMLParagraphElement(*getNode()))
    return ParagraphRole;

  if (isHTMLLabelElement(*getNode()))
    return LabelRole;

  if (isHTMLLegendElement(*getNode()))
    return LegendRole;

  if (isHTMLRubyElement(*getNode()))
    return RubyRole;

  if (isHTMLDListElement(*getNode()))
    return DescriptionListRole;

  if (isHTMLAudioElement(*getNode()))
    return AudioRole;
  if (isHTMLVideoElement(*getNode()))
    return VideoRole;

  if (getNode()->hasTagName(ddTag))
    return DescriptionListDetailRole;

  if (getNode()->hasTagName(dtTag))
    return DescriptionListTermRole;

  if (getNode()->nodeName() == "math")
    return MathRole;

  if (getNode()->hasTagName(rpTag) || getNode()->hasTagName(rtTag))
    return AnnotationRole;

  if (isHTMLFormElement(*getNode()))
    return FormRole;

  if (getNode()->hasTagName(abbrTag))
    return AbbrRole;

  if (getNode()->hasTagName(articleTag))
    return ArticleRole;

  if (getNode()->hasTagName(mainTag))
    return MainRole;

  if (getNode()->hasTagName(markTag))
    return MarkRole;

  if (getNode()->hasTagName(navTag))
    return NavigationRole;

  if (getNode()->hasTagName(asideTag))
    return ComplementaryRole;

  if (getNode()->hasTagName(preTag))
    return PreRole;

  if (getNode()->hasTagName(sectionTag))
    return RegionRole;

  if (getNode()->hasTagName(addressTag))
    return ContentInfoRole;

  if (isHTMLDialogElement(*getNode()))
    return DialogRole;

  if (isHTMLHtmlElement(*getNode()))
    return IgnoredRole;

  if (isHTMLIFrameElement(*getNode())) {
    const AtomicString& ariaRole =
        getAOMPropertyOrARIAAttribute(AOMStringProperty::kRole);
    if (ariaRole == "none" || ariaRole == "presentation")
      return IframePresentationalRole;
    return IframeRole;
  }

  if (getNode()->hasTagName(headerTag)) {
    if (isDescendantOfElementType(getLandmarkRolesNotAllowed()))
      return GroupRole;
    return BannerRole;
  }

  if (getNode()->hasTagName(footerTag)) {
    if (isDescendantOfElementType(getLandmarkRolesNotAllowed()))
      return GroupRole;
    return FooterRole;
  }

  if (getNode()->hasTagName(blockquoteTag))
    return BlockquoteRole;

  if (getNode()->hasTagName(captionTag))
    return CaptionRole;

  if (getNode()->hasTagName(figcaptionTag))
    return FigcaptionRole;

  if (getNode()->hasTagName(figureTag))
    return FigureRole;

  if (getNode()->nodeName() == "TIME")
    return TimeRole;

  if (isEmbeddedObject())
    return EmbeddedObjectRole;

  if (isHTMLHRElement(*getNode()))
    return SplitterRole;

  if (isFieldset())
    return GroupRole;

  return UnknownRole;
}
