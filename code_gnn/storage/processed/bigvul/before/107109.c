void tst_QQuickWebView::prepareWebViewComponent()
{
    static QDeclarativeEngine* engine = new QDeclarativeEngine(this);
    engine->addImportPath(QString::fromUtf8(IMPORT_DIR));

    m_component.reset(new QDeclarativeComponent(engine, this));

    m_component->setData(QByteArrayLiteral("import QtQuick 2.0\n"
                                           "import QtWebKit 3.0\n"
                                           "WebView {}")
                         , QUrl());
}
