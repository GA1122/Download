void HTMLFormControlElement::updateAncestorDisabledState() const
{
    HTMLFieldSetElement* fieldSetAncestor = 0;
    ContainerNode* legendAncestor = 0;
    for (HTMLElement* ancestor = Traversal<HTMLElement>::firstAncestor(*this); ancestor; ancestor = Traversal<HTMLElement>::firstAncestor(*ancestor)) {
        if (!legendAncestor && isHTMLLegendElement(*ancestor))
            legendAncestor = ancestor;
        if (isHTMLFieldSetElement(*ancestor)) {
            fieldSetAncestor = toHTMLFieldSetElement(ancestor);
            break;
        }
    }
    m_ancestorDisabledState = (fieldSetAncestor && fieldSetAncestor->isDisabledFormControl() && !(legendAncestor && legendAncestor == fieldSetAncestor->legend())) ? AncestorDisabledStateDisabled : AncestorDisabledStateEnabled;
}
