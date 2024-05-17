PassRefPtr<Element> Document::createElement(const AtomicString& localName, const AtomicString& typeExtension, ExceptionCode& ec)
{
    if (!isValidName(localName)) {
        ec = INVALID_CHARACTER_ERR;
        return 0;
    }

    if (m_registry) {
        if (PassRefPtr<Element> created = m_registry->createElement(QualifiedName(nullAtom, localName, xhtmlNamespaceURI), typeExtension))
            return created;
    }

    return setTypeExtension(createElement(localName, ec), typeExtension);
}
