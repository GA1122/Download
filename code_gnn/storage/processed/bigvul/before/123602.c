static Vector<Resource*> cachedResourcesForFrame(Frame* frame)
{
    Vector<Resource*> result;
    Document* rootDocument = frame->document();

    if (HTMLImport* rootImport = rootDocument->import()) {
        for (HTMLImport* import = rootImport; import; import = traverseNext(import)) {
            if (import->ownsLoader() || import->isRoot()) {
                if (Document* document = import->document())
                    cachedResourcesForDocument(document, result);
            }
        }
    } else {
        cachedResourcesForDocument(rootDocument, result);
    }

    return result;
}
