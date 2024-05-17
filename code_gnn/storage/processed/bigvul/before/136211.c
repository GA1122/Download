void PageSerializer::serialize(Page* page)
{
    serializeFrame(page->deprecatedLocalMainFrame());
}
