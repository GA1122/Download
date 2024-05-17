inline HTMLLinkElement::HTMLLinkElement(Document& document,
                                        const CreateElementFlags flags)
    : HTMLElement(linkTag, document),
      link_loader_(LinkLoader::Create(this)),
      referrer_policy_(kReferrerPolicyDefault),
      sizes_(DOMTokenList::Create(*this, HTMLNames::sizesAttr)),
      rel_list_(RelList::Create(this)),
      created_by_parser_(flags.IsCreatedByParser()) {}
