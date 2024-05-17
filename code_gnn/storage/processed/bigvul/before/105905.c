JSC::JSValue JSWorker::postMessage(JSC::ExecState* exec)
{
    return handlePostMessage(exec, impl());
}
