QQuickUrlSchemeDelegate* QQuickWebViewExperimental::schemeDelegates_At(QDeclarativeListProperty<QQuickUrlSchemeDelegate>* property, int index)
{
    const QObjectList children = property->object->children();
    if (index < children.count())
        return static_cast<QQuickUrlSchemeDelegate*>(children.at(index));
    return 0;
}
