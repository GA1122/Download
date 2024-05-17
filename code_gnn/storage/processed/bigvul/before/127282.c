static PassRefPtr<SharedBuffer> serializePageToMHTML(Page* page, MHTMLArchive::EncodingPolicy encodingPolicy)
{
    Vector<SerializedResource> resources;
    PageSerializer serializer(&resources);
    serializer.serialize(page);
    Document* document = page->mainFrame()->document();
    return MHTMLArchive::generateMHTMLData(resources, encodingPolicy, document->title(), document->suggestedMIMEType());
}
