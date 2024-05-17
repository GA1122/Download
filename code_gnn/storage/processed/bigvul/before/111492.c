void InputHandler::redrawSpellCheckDialogIfRequired(const bool shouldMoveDialog)
{
    if (didSpellCheckWord()) {
        imf_sp_text_t spellCheckingOptionRequest;
        spellCheckingOptionRequest.startTextPosition = 0;
        spellCheckingOptionRequest.endTextPosition = 0;
        WebCore::IntSize screenOffset(-1, -1);
        requestSpellingCheckingOptions(spellCheckingOptionRequest, screenOffset, shouldMoveDialog);
    }
}
