bool V8Proxy::handleOutOfMemory()
{
    v8::Local<v8::Context> context = v8::Context::GetCurrent();

    if (!context->HasOutOfMemoryException())
        return false;

    Frame* frame = V8Proxy::retrieveFrame(context);

    V8Proxy* proxy = V8Proxy::retrieve(frame);
    if (proxy) {
        proxy->clearForClose();

        proxy->windowShell()->destroyGlobal();
    }

#if PLATFORM(CHROMIUM)
    PlatformSupport::notifyJSOutOfMemory(frame);
#endif

    Settings* settings = frame->settings();
    ASSERT(settings);
    settings->setScriptEnabled(false);

    return true;
}
