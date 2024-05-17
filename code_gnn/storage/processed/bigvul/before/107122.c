PassRefPtr<LayerTreeHostQt> LayerTreeHostQt::create(WebPage* webPage)
{
    return adoptRef(new LayerTreeHostQt(webPage));
}
