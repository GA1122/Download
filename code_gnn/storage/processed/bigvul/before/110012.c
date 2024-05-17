void HTMLSelectElement::finishParsingChildren()
{
    HTMLFormControlElementWithState::finishParsingChildren();
    m_isParsingInProgress = false;
    updateListItemSelectedStates();
}
