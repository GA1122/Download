Node* Document::cloneNode(bool deep)
{
    Document* clone = cloneDocumentWithoutChildren();
    clone->cloneDataFromDocument(*this);
    if (deep)
        cloneChildNodes(clone);
    return clone;
}
