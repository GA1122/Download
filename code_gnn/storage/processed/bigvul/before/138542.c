void initialize(Platform* platform)
{
    initializeWithoutV8(platform);

    V8Initializer::initializeMainThreadIfNeeded();

    s_isolateInterruptor = new V8IsolateInterruptor(V8PerIsolateData::mainThreadIsolate());
    ThreadState::current()->addInterruptor(s_isolateInterruptor);
    ThreadState::current()->registerTraceDOMWrappers(V8PerIsolateData::mainThreadIsolate(), V8GCController::traceDOMWrappers);

    if (WebThread* currentThread = platform->currentThread()) {
        ASSERT(!s_endOfTaskRunner);
        s_endOfTaskRunner = new EndOfTaskRunner;
        currentThread->addTaskObserver(s_endOfTaskRunner);
    }
}
