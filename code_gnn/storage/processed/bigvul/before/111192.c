bool WebPage::isMaxZoomed() const
{
    return (d->currentScale() == d->maximumScale()) || !d->isUserScalable();
}
