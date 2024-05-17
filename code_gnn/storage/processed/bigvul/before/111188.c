bool WebPage::isAtInitialZoom() const
{
    return (d->currentScale() == d->initialScale()) || !d->isUserScalable();
}
