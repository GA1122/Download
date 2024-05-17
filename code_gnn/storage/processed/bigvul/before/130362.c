bool HTMLFormControlElement::reportValidity()
{
    WillBeHeapVector<RefPtrWillBeMember<HTMLFormControlElement>> unhandledInvalidControls;
    bool isValid = checkValidity(&unhandledInvalidControls, CheckValidityDispatchInvalidEvent);
    if (isValid || unhandledInvalidControls.isEmpty())
        return isValid;
    ASSERT(unhandledInvalidControls.size() == 1);
    ASSERT(unhandledInvalidControls[0].get() == this);
    document().updateLayoutIgnorePendingStylesheets();
    if (isFocusable()) {
        showValidationMessage();
        return false;
    }
    if (document().frame()) {
        String message("An invalid form control with name='%name' is not focusable.");
        message.replace("%name", name());
        document().addConsoleMessage(ConsoleMessage::create(RenderingMessageSource, ErrorMessageLevel, message));
    }
    return false;
}
