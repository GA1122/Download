void InputHandler::learnText()
{
    if (!isActiveTextEdit())
        return;

    if (m_currentFocusElementTextEditMask & NO_PREDICTION || m_currentFocusElementTextEditMask & NO_AUTO_TEXT)
        return;

    WTF::String textInField(elementText());
    textInField = textInField.substring(std::max(0, static_cast<int>(textInField.length() - MaxLearnTextDataSize)), textInField.length());
    textInField = textInField.stripWhiteSpace();

    ASSERT(textInField.length() <= MaxLearnTextDataSize);

    if (textInField.isEmpty())
        return;

    InputLog(LogLevelInfo, "InputHandler::learnText '%s'", textInField.latin1().data());
    sendLearnTextDetails(textInField);
}
