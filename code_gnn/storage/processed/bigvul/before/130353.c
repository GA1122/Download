String HTMLFormControlElement::formAction() const
{
    const AtomicString& action = fastGetAttribute(formactionAttr);
    if (action.isEmpty())
        return document().url();
    return document().completeURL(stripLeadingAndTrailingHTMLSpaces(action));
}
