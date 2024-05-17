    WrapperWindow(QQuickWebView* view)
        : QQuickView(QUrl("data:text/plain,import QtQuick 2.0\nItem { objectName: 'root' }"))
