bool HTMLSelectElement::isPresentationAttribute(const QualifiedName& name) const
{
    if (name == alignAttr) {
        return false;
    }

    return HTMLFormControlElementWithState::isPresentationAttribute(name);
}
