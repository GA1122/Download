SpellChecker* InputHandler::getSpellChecker()
{
    if (!m_currentFocusElement || !m_currentFocusElement->document())
        return 0;

    if (Frame* frame = m_currentFocusElement->document()->frame())
        if (Editor* editor = frame->editor())
            return editor->spellChecker();

     return 0;
 }
