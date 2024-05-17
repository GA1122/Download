void InputHandler::ensureFocusElementVisible(bool centerInView)
{
    if (isActivePlugin())
        ensureFocusPluginElementVisible();
    else
        ensureFocusTextElementVisible(centerInView ? CenterAlways : CenterIfNeeded);
}
