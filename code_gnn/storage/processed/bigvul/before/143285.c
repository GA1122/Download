Document& Document::topDocument() const
{
    Document* doc = const_cast<Document*>(this);
    for (HTMLFrameOwnerElement* element = doc->localOwner(); element; element = doc->localOwner())
        doc = &element->document();

    DCHECK(doc);
    return *doc;
}
