HTMLCanvasElement& Document::getCSSCanvasElement(const String& name)
{
    RefPtrWillBeMember<HTMLCanvasElement>& element = m_cssCanvasElements.add(name, nullptr).storedValue->value;
    if (!element) {
        element = HTMLCanvasElement::create(*this);
        element->setAccelerationDisabled(true);
    }
    return *element;
}
