const HTMLElement& toHTMLElement(const FormAssociatedElement& associatedElement)
{
    if (associatedElement.isFormControlElement())
        return toHTMLFormControlElement(associatedElement);
    return toHTMLObjectElement(associatedElement);
}
