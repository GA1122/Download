void Document::didCreateCustomElement(Element* element, CustomElementConstructor* constructor)
{
    if (m_registry)
        m_registry->didCreateElement(element);
}
