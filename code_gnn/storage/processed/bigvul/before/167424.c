inline HTMLStyleElement::HTMLStyleElement(Document& document,
                                          const CreateElementFlags flags)
    : HTMLElement(styleTag, document),
      StyleElement(&document, flags.IsCreatedByParser()),
      fired_load_(false),
      loaded_sheet_(false) {}
