void shutdown()
{
    if (Platform::current()->currentThread()) {
        delete s_endOfTaskRunner;
        s_endOfTaskRunner = 0;
    }

    ASSERT(s_isolateInterruptor);
    ThreadState::current()->removeInterruptor(s_isolateInterruptor);

    if (Platform::current()->currentThread()) {
        ASSERT(s_pendingGCRunner);
        delete s_pendingGCRunner;
        s_pendingGCRunner = 0;

        ASSERT(s_messageLoopInterruptor);
        ThreadState::current()->removeInterruptor(s_messageLoopInterruptor);
        delete s_messageLoopInterruptor;
        s_messageLoopInterruptor = 0;
    }

    ScriptStreamerThread::shutdown();

    v8::Isolate* isolate = V8PerIsolateData::mainThreadIsolate();
    V8PerIsolateData::willBeDestroyed(isolate);

    WorkerThread::terminateAndWaitForAllWorkers();

    ModulesInitializer::terminateThreads();

    ThreadState::detachMainThread();

    V8PerIsolateData::destroy(isolate);

    shutdownWithoutV8();
}
