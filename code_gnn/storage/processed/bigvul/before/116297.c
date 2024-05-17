void QQuickWebViewExperimental::setColorChooser(QQmlComponent* colorChooser)
{
    Q_D(QQuickWebView);
    if (d->colorChooser == colorChooser)
        return;

    d->colorChooser = colorChooser;
    emit colorChooserChanged();
}
