YR_ARENA_PAGE* _yr_arena_page_for_address(
    YR_ARENA* arena,
    void* address)
{
  YR_ARENA_PAGE* page;


  page = arena->current_page;

  if (page != NULL &&
      (uint8_t*) address >= page->address &&
      (uint8_t*) address < page->address + page->used)
    return page;

  page = arena->page_list_head;

  while (page != NULL)
  {
    if ((uint8_t*) address >= page->address &&
        (uint8_t*) address < page->address + page->used)
      return page;
    page = page->next;
  }

  return NULL;
}
