DEFINE_TRACE(NavigatorServiceWorker)
{
    visitor->trace(m_serviceWorker);
    HeapSupplement<Navigator>::trace(visitor);
    DOMWindowProperty::trace(visitor);
}
