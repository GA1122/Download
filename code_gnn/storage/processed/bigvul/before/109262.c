DocumentLoader* InspectorPageAgent::assertDocumentLoader(ErrorString* errorString, Frame* frame)
{
    DocumentLoader* documentLoader = frame->loader().documentLoader();
    if (!documentLoader)
        *errorString = "No documentLoader for given frame found";
    return documentLoader;
}
