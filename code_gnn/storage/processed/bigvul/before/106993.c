void QQuickWebViewPrivate::execDialogRunner(QtDialogRunner& dialogRunner)
{
    setViewInAttachedProperties(dialogRunner.dialog());

    disableMouseEvents();
    m_dialogRunnerActive = true;

    dialogRunner.exec();
    m_dialogRunnerActive = false;
    enableMouseEvents();
}
