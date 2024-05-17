void HTMLInputElement::defaultEventHandler(Event* evt)
{
    if (evt->isMouseEvent() && evt->type() == eventNames().clickEvent && static_cast<MouseEvent*>(evt)->button() == LeftButton) {
        m_inputType->handleClickEvent(static_cast<MouseEvent*>(evt));
        if (evt->defaultHandled())
            return;
    }

    if (evt->isTouchEvent()) {
        m_inputType->handleTouchEvent(static_cast<TouchEvent*>(evt));
        if (evt->defaultHandled())
            return;
    }

    if (evt->isKeyboardEvent() && evt->type() == eventNames().keydownEvent) {
        m_inputType->handleKeydownEvent(static_cast<KeyboardEvent*>(evt));
        if (evt->defaultHandled())
            return;
    }

    bool callBaseClassEarly = isTextField() && (evt->type() == eventNames().keydownEvent || evt->type() == eventNames().keypressEvent);
    if (callBaseClassEarly) {
        HTMLTextFormControlElement::defaultEventHandler(evt);
        if (evt->defaultHandled())
            return;
    }

    if (evt->type() == eventNames().DOMActivateEvent) {
        m_inputType->handleDOMActivateEvent(evt);
        if (evt->defaultHandled())
            return;
    }

    if (evt->isKeyboardEvent() && evt->type() == eventNames().keypressEvent) {
        m_inputType->handleKeypressEvent(static_cast<KeyboardEvent*>(evt));
        if (evt->defaultHandled())
            return;
    }

    if (evt->isKeyboardEvent() && evt->type() == eventNames().keyupEvent) {
        m_inputType->handleKeyupEvent(static_cast<KeyboardEvent*>(evt));
        if (evt->defaultHandled())
            return;
    }

    if (m_inputType->shouldSubmitImplicitly(evt)) {
        if (isSearchField())
            onSearch();
        if (wasChangedSinceLastFormControlChangeEvent())
            dispatchFormControlChangeEvent();

        RefPtr<HTMLFormElement> formForSubmission = m_inputType->formForSubmission();
        if (formForSubmission)
            formForSubmission->submitImplicitly(evt, canTriggerImplicitSubmission());

        evt->setDefaultHandled();
        return;
    }

    if (evt->isBeforeTextInsertedEvent())
        m_inputType->handleBeforeTextInsertedEvent(static_cast<BeforeTextInsertedEvent*>(evt));

    if (evt->isMouseEvent() && evt->type() == eventNames().mousedownEvent) {
        m_inputType->handleMouseDownEvent(static_cast<MouseEvent*>(evt));
        if (evt->defaultHandled())
            return;
    }

    m_inputType->forwardEvent(evt);

    if (!callBaseClassEarly && !evt->defaultHandled())
        HTMLTextFormControlElement::defaultEventHandler(evt);
}
