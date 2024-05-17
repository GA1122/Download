void HTMLSelectElement::add(HTMLElement* element, HTMLElement* before, ExceptionCode& ec)
{
    RefPtr<HTMLElement> protectNewChild(element);

    if (!element || !(element->hasLocalName(optionTag) || element->hasLocalName(hrTag)))
        return;

    insertBefore(element, before, ec, AttachLazily);
    setNeedsValidityCheck();
}
