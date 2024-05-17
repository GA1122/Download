void SVGStyleElement::ParseAttribute(
    const AttributeModificationParams& params) {
  if (params.name == SVGNames::titleAttr) {
    if (sheet_ && IsInDocumentTree())
      sheet_->SetTitle(params.new_value);

    return;
  }

  SVGElement::ParseAttribute(params);
}
