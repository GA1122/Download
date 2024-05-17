PassRefPtr<TypeBuilder::Page::FrameResourceTree> InspectorPageAgent::buildObjectForFrameTree(Frame* frame)
{
    RefPtr<TypeBuilder::Page::Frame> frameObject = buildObjectForFrame(frame);
    RefPtr<TypeBuilder::Array<TypeBuilder::Page::FrameResourceTree::Resources> > subresources = TypeBuilder::Array<TypeBuilder::Page::FrameResourceTree::Resources>::create();
    RefPtr<TypeBuilder::Page::FrameResourceTree> result = TypeBuilder::Page::FrameResourceTree::create()
         .setFrame(frameObject)
         .setResources(subresources);

    Vector<Resource*> allResources = cachedResourcesForFrame(frame);
    for (Vector<Resource*>::const_iterator it = allResources.begin(); it != allResources.end(); ++it) {
        Resource* cachedResource = *it;

        RefPtr<TypeBuilder::Page::FrameResourceTree::Resources> resourceObject = TypeBuilder::Page::FrameResourceTree::Resources::create()
            .setUrl(urlWithoutFragment(cachedResource->url()).string())
            .setType(cachedResourceTypeJson(*cachedResource))
            .setMimeType(cachedResource->response().mimeType());
        if (cachedResource->wasCanceled())
            resourceObject->setCanceled(true);
        else if (cachedResource->status() == Resource::LoadError)
            resourceObject->setFailed(true);
        subresources->addItem(resourceObject);
    }

    Vector<HTMLImportChild*> allImports = importsForFrame(frame);
    for (Vector<HTMLImportChild*>::const_iterator it = allImports.begin(); it != allImports.end(); ++it) {
        HTMLImportChild* import = *it;
        RefPtr<TypeBuilder::Page::FrameResourceTree::Resources> resourceObject = TypeBuilder::Page::FrameResourceTree::Resources::create()
            .setUrl(urlWithoutFragment(import->url()).string())
            .setType(resourceTypeJson(InspectorPageAgent::DocumentResource))
            .setMimeType(import->document()->suggestedMIMEType());
        subresources->addItem(resourceObject);
    }

    RefPtr<TypeBuilder::Array<TypeBuilder::Page::FrameResourceTree> > childrenArray;
    for (Frame* child = frame->tree().firstChild(); child; child = child->tree().nextSibling()) {
        if (!childrenArray) {
            childrenArray = TypeBuilder::Array<TypeBuilder::Page::FrameResourceTree>::create();
            result->setChildFrames(childrenArray);
        }
        childrenArray->addItem(buildObjectForFrameTree(child));
    }
    return result;
}
