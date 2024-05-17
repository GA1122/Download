void initializeWithoutV8(Platform* platform)
{
    ASSERT(!s_webKitInitialized);
    s_webKitInitialized = true;

    ASSERT(platform);
    Platform::initialize(platform);

    WTF::setRandomSource(cryptographicallyRandomValues);
    WTF::initialize(currentTimeFunction, monotonicallyIncreasingTimeFunction, systemTraceTimeFunction, histogramEnumerationFunction);
    WTF::initializeMainThread(callOnMainThreadFunction);
    Heap::init();

    ThreadState::attachMainThread();
    if (WebThread* currentThread = platform->currentThread()) {
        ASSERT(!s_pendingGCRunner);
        s_pendingGCRunner = new PendingGCRunner;
        currentThread->addTaskObserver(s_pendingGCRunner);

        ASSERT(!s_messageLoopInterruptor);
        s_messageLoopInterruptor = new MessageLoopInterruptor(currentThread);
        ThreadState::current()->addInterruptor(s_messageLoopInterruptor);
    }

    DEFINE_STATIC_LOCAL(ModulesInitializer, initializer, ());
    initializer.init();

    setIndexedDBClientCreateFunction(IndexedDBClientImpl::create);

    MediaPlayer::setMediaEngineCreateFunction(WebMediaPlayerClientImpl::create);
}
