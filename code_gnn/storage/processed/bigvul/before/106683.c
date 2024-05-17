ULONG STDMETHODCALLTYPE WebView::Release(void)
{
    deref();
    return refCount();
}
