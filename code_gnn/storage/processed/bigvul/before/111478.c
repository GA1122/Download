bool InputHandler::isCaretAtEndOfText()
{
    return caretPosition() == static_cast<int>(elementText().length());
}
