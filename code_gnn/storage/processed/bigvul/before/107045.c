QWebPreferences* QQuickWebViewExperimental::preferences() const
{
    QQuickWebViewPrivate* const d = d_ptr;
    if (!d->preferences)
        d->preferences = adoptPtr(QWebPreferencesPrivate::createPreferences(d));
    return d->preferences.get();
}
