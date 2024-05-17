bool WebPage::isMinZoomed() const
{
    return (d->currentScale() == d->minimumScale()) || !d->isUserScalable();
}
