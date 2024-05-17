void InspectorPageAgent::willRunJavaScriptDialog(const String& message)
{
    m_frontend->javascriptDialogOpening(message);
}
