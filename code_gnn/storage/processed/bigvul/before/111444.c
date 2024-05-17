int InputHandler::caretPosition() const
{
    if (!isActiveTextEdit())
        return -1;

    return selectionStart();
}
