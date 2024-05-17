void InspectorPageAgent::getScriptExecutionStatus(ErrorString*, PageCommandHandler::Result::Enum* status)
{
    bool disabledByScriptController = false;
    bool disabledInSettings = false;
    Frame* frame = mainFrame();
    if (frame) {
        disabledByScriptController = !frame->script().canExecuteScripts(NotAboutToExecuteScript);
        if (frame->settings())
            disabledInSettings = !frame->settings()->isScriptEnabled();
    }

    if (!disabledByScriptController) {
        *status = PageCommandHandler::Result::Allowed;
        return;
    }

    if (disabledInSettings)
        *status = PageCommandHandler::Result::Disabled;
    else
        *status = PageCommandHandler::Result::Forbidden;
}
