inline SVGStyleElement::SVGStyleElement(Document& document,
                                        const CreateElementFlags flags)
    : SVGElement(SVGNames::styleTag, document),
      StyleElement(&document, flags.IsCreatedByParser()) {}
