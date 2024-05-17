void PageSerializer::serialize(Page* page)
{
    serializeFrame(page->mainFrame());
}
