bool InputHandler::removeComposedText()
{
    if (compositionActive()) {
        if (!deleteText(m_composingTextStart, m_composingTextEnd)) {
            return false;
        }
        removeAttributedTextMarker();
    }

    return true;
}
