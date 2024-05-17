QQmlListProperty<QQuickUrlSchemeDelegate> QQuickWebViewExperimental::schemeDelegates()
{
    return QQmlListProperty<QQuickUrlSchemeDelegate>(schemeParent, 0,
            QQuickWebViewExperimental::schemeDelegates_Append,
            QQuickWebViewExperimental::schemeDelegates_Count,
            QQuickWebViewExperimental::schemeDelegates_At,
            QQuickWebViewExperimental::schemeDelegates_Clear);
}
