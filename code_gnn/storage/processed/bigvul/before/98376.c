gsize webkit_gc_count_javascript_objects()
{
    JSC::JSLock lock(JSC::SilenceAssertionsOnly);
    return JSDOMWindow::commonJSGlobalData()->heap.objectCount();

}
