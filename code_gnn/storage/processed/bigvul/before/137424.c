NavigatorServiceWorker* NavigatorServiceWorker::toNavigatorServiceWorker(Navigator& navigator)
{
    return static_cast<NavigatorServiceWorker*>(HeapSupplement<Navigator>::from(navigator, supplementName()));
}
