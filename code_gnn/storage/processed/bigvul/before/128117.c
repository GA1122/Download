void SpinLock::lockSlow()
{
    static const int kYieldProcessorTries = 1000;
    do {
        do {
            for (int count = 0; count < kYieldProcessorTries; ++count) {
                YIELD_PROCESSOR;
                if (!m_lock.load(std::memory_order_relaxed) && LIKELY(!m_lock.exchange(true, std::memory_order_acq_rel)))
                    return;
            }

            YIELD_THREAD;
        } while (m_lock.load(std::memory_order_relaxed));
    } while (UNLIKELY(m_lock.exchange(true, std::memory_order_acq_rel)));
}
