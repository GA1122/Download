NavigatorServiceWorker* NavigatorServiceWorker::from(Document& document)
{
    if (!document.frame() || !document.frame()->domWindow())
        return nullptr;
    Navigator& navigator = *document.frame()->domWindow()->navigator();
    return &from(navigator);
}
