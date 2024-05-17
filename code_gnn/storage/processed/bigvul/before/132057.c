LayoutBlockFlow* LayoutBlockFlow::createAnonymous(Document* document)
{
    LayoutBlockFlow* layoutBlockFlow = new LayoutBlockFlow(nullptr);
    layoutBlockFlow->setDocumentForAnonymous(document);
    return layoutBlockFlow;
}
