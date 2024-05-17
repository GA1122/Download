LargeObjectPage::LargeObjectPage(PageMemory* storage,
                                 BaseArena* arena,
                                 size_t payloadSize)
    : BasePage(storage, arena),
      m_payloadSize(payloadSize)
#if ENABLE(ASAN_CONTAINER_ANNOTATIONS)
      ,
      m_isVectorBackingPage(false)
#endif
{
}
