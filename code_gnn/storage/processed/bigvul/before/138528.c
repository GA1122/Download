void WorkerThread::willDestroyIsolate()
{
    ASSERT(isCurrentThread());
    ASSERT(m_isolate);
    V8PerIsolateData::willBeDestroyed(m_isolate);
    ThreadState::current()->removeInterruptor(m_interruptor.get());
}
