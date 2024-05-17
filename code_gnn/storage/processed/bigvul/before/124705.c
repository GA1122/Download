RenderBlockFlow* RenderBlockFlow::createAnonymous(Document* document)
{
    RenderBlockFlow* renderer = new RenderBlockFlow(0);
    renderer->setDocumentForAnonymous(document);
    return renderer;
}
