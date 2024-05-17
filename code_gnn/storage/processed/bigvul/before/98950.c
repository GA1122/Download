void HTMLLinkElement::parseMappedAttribute(Attribute* attr)
{
    if (attr->name() == relAttr) {
        tokenizeRelAttribute(attr->value(), m_relAttribute);
        process();
    } else if (attr->name() == hrefAttr) {
        m_url = document()->completeURL(deprecatedParseURL(attr->value()));
        process();
    } else if (attr->name() == typeAttr) {
        m_type = attr->value();
        process();
    } else if (attr->name() == mediaAttr) {
        m_media = attr->value().string().lower();
        process();
    } else if (attr->name() == disabledAttr)
        setDisabledState(!attr->isNull());
    else if (attr->name() == onbeforeloadAttr)
        setAttributeEventListener(eventNames().beforeloadEvent, createAttributeEventListener(this, attr));
    else {
        if (attr->name() == titleAttr && m_sheet)
            m_sheet->setTitle(attr->value());
        HTMLElement::parseMappedAttribute(attr);
    }
}
