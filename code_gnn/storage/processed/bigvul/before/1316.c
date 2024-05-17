void* DynamicMetadataProvider::cleanup_fn(void* pv)
{
    DynamicMetadataProvider* provider = reinterpret_cast<DynamicMetadataProvider*>(pv);

#ifndef WIN32
    Thread::mask_all_signals();
#endif

    if (!provider->m_id.empty()) {
        string threadid("[");
        threadid += provider->m_id + ']';
        logging::NDC::push(threadid);
    }

#ifdef _DEBUG
    xmltooling::NDC ndc("cleanup");
#endif

    auto_ptr<Mutex> mutex(Mutex::create());
    mutex->lock();

    Category& log = Category::getInstance(SAML_LOGCAT ".MetadataProvider.Dynamic");

    log.info("cleanup thread started...running every %d seconds", provider->m_cleanupInterval);

    while (!provider->m_shutdown) {
        provider->m_cleanup_wait->timedwait(mutex.get(), provider->m_cleanupInterval);
        if (provider->m_shutdown)
            break;

        log.info("cleaning dynamic metadata cache...");

        provider->m_lock->wrlock();
        SharedLock locker(provider->m_lock, false);

        time_t now = time(nullptr);
        for (map<xstring, time_t>::iterator i = provider->m_cacheMap.begin(), i2 = i; i != provider->m_cacheMap.end(); i = i2) {
            ++i2;
            if (now > i->second + provider->m_cleanupTimeout) {
                if (log.isDebugEnabled()) {
                    auto_ptr_char id(i->first.c_str());
                    log.debug("removing cache entry for (%s)", id.get());
                }
                provider->unindex(i->first.c_str(), true);
                provider->m_cacheMap.erase(i);
            }
        }
    }

    log.info("cleanup thread finished");

    mutex->unlock();

    if (!provider->m_id.empty()) {
        logging::NDC::pop();
    }

    return nullptr;
}
