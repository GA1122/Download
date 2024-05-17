Document& Document::topDocument() const
{
    Document* doc = const_cast<Document*>(this);
    for (HTMLFrameOwnerElement* element = doc->ownerElement(); element; element = doc->ownerElement())
        doc = &element->document();

    ASSERT(doc);
    return *doc;
}
