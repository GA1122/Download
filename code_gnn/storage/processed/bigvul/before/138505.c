void WorkerThread::initialize(PassOwnPtr<WorkerThreadStartupData> startupData)
{
    KURL scriptURL = startupData->m_scriptURL;
    String sourceCode = startupData->m_sourceCode;
    WorkerThreadStartMode startMode = startupData->m_startMode;
    OwnPtr<Vector<char>> cachedMetaData = startupData->m_cachedMetaData.release();
    V8CacheOptions v8CacheOptions = startupData->m_v8CacheOptions;

    m_webScheduler = backingThread().platformThread().scheduler();
    {
        MutexLocker lock(m_threadStateMutex);

        if (m_terminated) {
            m_workerReportingProxy.workerThreadTerminated();
            return;
        }

        m_microtaskRunner = adoptPtr(new WorkerMicrotaskRunner(this));
        backingThread().addTaskObserver(m_microtaskRunner.get());
        backingThread().initialize();

        m_isolate = initializeIsolate();
        m_workerGlobalScope = createWorkerGlobalScope(startupData);
        m_workerGlobalScope->scriptLoaded(sourceCode.length(), cachedMetaData.get() ? cachedMetaData->size() : 0);

        PlatformThreadData::current().threadTimers().setSharedTimer(adoptPtr(new WorkerSharedTimer(this)));
    }

    didStartRunLoop();

    m_workerReportingProxy.workerGlobalScopeStarted(m_workerGlobalScope.get());

    WorkerScriptController* script = m_workerGlobalScope->script();
    if (!script->isExecutionForbidden())
        script->initializeContextIfNeeded();
    if (startMode == PauseWorkerGlobalScopeOnStart)
        m_workerGlobalScope->workerInspectorController()->pauseOnStart();

    OwnPtr<CachedMetadataHandler> handler(workerGlobalScope()->createWorkerScriptCachedMetadataHandler(scriptURL, cachedMetaData.get()));
    bool success = script->evaluate(ScriptSourceCode(sourceCode, scriptURL), nullptr, handler.get(), v8CacheOptions);
    m_workerGlobalScope->didEvaluateWorkerScript();
    m_workerReportingProxy.didEvaluateWorkerScript(success);

    postInitialize();

    m_webScheduler->postIdleTaskAfterWakeup(FROM_HERE, WTF::bind<double>(&WorkerThread::performIdleWork, this));
}
