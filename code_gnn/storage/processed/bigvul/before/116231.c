void QQuickWebViewPrivate::chooseFiles(WKOpenPanelResultListenerRef listenerRef, const QStringList& selectedFileNames, QtWebPageUIClient::FileChooserType type)
{
    Q_Q(QQuickWebView);

    QtDialogRunner dialogRunner(q);
    if (!dialogRunner.initForFilePicker(selectedFileNames, (type == QtWebPageUIClient::MultipleFilesSelection)))
        return;

    dialogRunner.run();

    if (dialogRunner.wasAccepted()) {
        QStringList selectedPaths = dialogRunner.filePaths();

        Vector<RefPtr<APIObject> > wkFiles(selectedPaths.size());
        for (unsigned i = 0; i < selectedPaths.size(); ++i)
            wkFiles[i] = WebURL::create(QUrl::fromLocalFile(selectedPaths.at(i)).toString());            

        WKOpenPanelResultListenerChooseFiles(listenerRef, toAPI(ImmutableArray::adopt(wkFiles).leakRef()));
    } else
        WKOpenPanelResultListenerCancel(listenerRef);

}
