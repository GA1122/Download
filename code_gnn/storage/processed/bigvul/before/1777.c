void IRCView::updateAppearance()
{
    if (Preferences::self()->customTextFont())
        setFont(Preferences::self()->textFont());
    else
        setFont(KGlobalSettings::generalFont());

    setVerticalScrollBarPolicy(Preferences::self()->showIRCViewScrollBar() ? Qt::ScrollBarAlwaysOn : Qt::ScrollBarAlwaysOff);

    if (Preferences::self()->showBackgroundImage())
    {
        KUrl url = Preferences::self()->backgroundImage();

        if (url.hasPath())
        {
            viewport()->setStyleSheet("QWidget { background-image: url("+url.path()+"); background-attachment:fixed; }");

            return;
        }
    }

    if (!viewport()->styleSheet().isEmpty())
        viewport()->setStyleSheet("");

    QPalette p;
    p.setColor(QPalette::Base, Preferences::self()->color(Preferences::TextViewBackground));
    viewport()->setPalette(p);
}
