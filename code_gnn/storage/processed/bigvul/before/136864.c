void HTMLInputElement::CollectStyleForPresentationAttribute(
    const QualifiedName& name,
    const AtomicString& value,
    MutableCSSPropertyValueSet* style) {
  if (name == vspaceAttr) {
    AddHTMLLengthToStyle(style, CSSPropertyMarginTop, value);
    AddHTMLLengthToStyle(style, CSSPropertyMarginBottom, value);
  } else if (name == hspaceAttr) {
    AddHTMLLengthToStyle(style, CSSPropertyMarginLeft, value);
    AddHTMLLengthToStyle(style, CSSPropertyMarginRight, value);
  } else if (name == alignAttr) {
    if (input_type_->ShouldRespectAlignAttribute())
      ApplyAlignmentAttributeToStyle(value, style);
  } else if (name == widthAttr) {
    if (input_type_->ShouldRespectHeightAndWidthAttributes())
      AddHTMLLengthToStyle(style, CSSPropertyWidth, value);
  } else if (name == heightAttr) {
    if (input_type_->ShouldRespectHeightAndWidthAttributes())
      AddHTMLLengthToStyle(style, CSSPropertyHeight, value);
  } else if (name == borderAttr &&
             type() == InputTypeNames::image) {   
    ApplyBorderAttributeToStyle(value, style);
  } else {
    TextControlElement::CollectStyleForPresentationAttribute(name, value,
                                                             style);
  }
}
