void Document::pluginLoadingTimerFired(Timer<Document>*)
{
    updateStyleAndLayout();
}
