LargeObjectPage::LargeObjectPage(PageMemory* storage,
                                 BaseArena* arena,
                                 size_t payload_size)
    : BasePage(storage, arena),
      payload_size_(payload_size)
#ifdef ANNOTATE_CONTIGUOUS_CONTAINER
      ,
      is_vector_backing_page_(false)
#endif
{
}
