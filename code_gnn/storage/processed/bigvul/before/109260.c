static Vector<KURL> allResourcesURLsForFrame(Frame* frame)
{
    Vector<KURL> result;

    result.append(urlWithoutFragment(frame->loader().documentLoader()->url()));

    Vector<Resource*> allResources = cachedResourcesForFrame(frame);
    for (Vector<Resource*>::const_iterator it = allResources.begin(); it != allResources.end(); ++it)
        result.append(urlWithoutFragment((*it)->url()));

    return result;
}
