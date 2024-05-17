    virtual void notifyNeedPage(void* page)
    {
#if USE(MADV_FREE_FOR_JIT_MEMORY)
        UNUSED_PARAM(page);
#else
        m_reservation.commit(page, pageSize());
#endif
    }
