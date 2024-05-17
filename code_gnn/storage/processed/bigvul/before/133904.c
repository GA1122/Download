const HTMLElement* toHTMLElement(const FormAssociatedElement* associatedElement)
{
    ASSERT(associatedElement);
    return &toHTMLElement(*associatedElement);
}
