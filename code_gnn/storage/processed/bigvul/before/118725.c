const AtomicString& HTMLDocument::bodyAttributeValue(const QualifiedName& name) const
{
    if (HTMLBodyElement* body = htmlBodyElement())
        return body->fastGetAttribute(name);
    return nullAtom;
}
