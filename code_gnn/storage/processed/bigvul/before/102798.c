    FixedVMPoolExecutableAllocator()
        : MetaAllocator(jitAllocationGranule)  
    {
        m_reservation = PageReservation::reserveWithGuardPages(fixedExecutableMemoryPoolSize, OSAllocator::JSJITCodePages, EXECUTABLE_POOL_WRITABLE, true);
#if !ENABLE(LLINT)
        if (!m_reservation)
            CRASH();
#endif
        if (m_reservation) {
            ASSERT(m_reservation.size() == fixedExecutableMemoryPoolSize);
            addFreshFreeSpace(m_reservation.base(), m_reservation.size());
            
            startOfFixedExecutableMemoryPool = reinterpret_cast<uintptr_t>(m_reservation.base());
        }
    }
