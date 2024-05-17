void DateTimeSymbolicFieldElement::handleKeyboardEvent(KeyboardEvent* keyboardEvent)
{
    if (keyboardEvent->type() != eventNames().keypressEvent)
        return;

    const UChar charCode = WTF::Unicode::toLower(keyboardEvent->charCode());
    if (charCode < ' ')
        return;

    keyboardEvent->setDefaultHandled();

    int index = m_typeAhead.handleEvent(keyboardEvent, TypeAhead::MatchPrefix | TypeAhead::CycleFirstChar | TypeAhead::MatchIndex);
    if (index < 0)
        return;
    setValueAsInteger(index, DispatchEvent);
}
