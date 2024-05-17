void InspectorPageAgent::didRunJavaScriptDialog()
{
    m_frontend->javascriptDialogClosed();
}
