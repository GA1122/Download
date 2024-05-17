    virtual void notifyPageIsFree(void* page)
    {
#if USE(MADV_FREE_FOR_JIT_MEMORY)
        for (;;) {
            int result = madvise(page, pageSize(), MADV_FREE);
            if (!result)
                return;
            ASSERT(result == -1);
            if (errno != EAGAIN) {
                ASSERT_NOT_REACHED();  
                break;  
            }
        }
#else
        m_reservation.decommit(page, pageSize());
#endif
    }
