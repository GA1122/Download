void DateTimeFieldElement::defaultKeyboardEventHandler(KeyboardEvent* keyboardEvent)
{
    if (keyboardEvent->type() != eventNames().keydownEvent)
        return;

    const String& keyIdentifier = keyboardEvent->keyIdentifier();

    if (keyIdentifier == "Down") {
        if (keyboardEvent->getModifierState("Alt"))
            return;
        keyboardEvent->setDefaultHandled();
        stepDown();
        return;
    }

    if (keyIdentifier == "Left") {
        if (!m_fieldOwner)
            return;
        if (!localeForOwner().isRTL() && m_fieldOwner->focusOnPreviousField(*this))
            keyboardEvent->setDefaultHandled();
        return;
    }

    if (keyIdentifier == "Right") {
        if (!m_fieldOwner)
            return;
        if (!localeForOwner().isRTL() && m_fieldOwner->focusOnNextField(*this))
            keyboardEvent->setDefaultHandled();
        return;
    }

    if (keyIdentifier == "Up") {
        keyboardEvent->setDefaultHandled();
        stepUp();
        return;
    }

    if (keyIdentifier == "U+0008" || keyIdentifier == "U+007F") {
        keyboardEvent->setDefaultHandled();
        setEmptyValue(DispatchEvent);
        return;
    }
}
