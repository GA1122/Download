void HTMLSelectElement::childrenChanged(bool changedByParser, Node* beforeChange, Node* afterChange, int childCountDelta)
{
    setRecalcListItems();
    setNeedsValidityCheck();
    m_lastOnChangeSelection.clear();

    HTMLFormControlElementWithState::childrenChanged(changedByParser, beforeChange, afterChange, childCountDelta);
}
