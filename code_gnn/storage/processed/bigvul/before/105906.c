JSC::JSValue JSWorker::webkitPostMessage(JSC::ExecState* exec)
{
    return handlePostMessage(exec, impl());
}
