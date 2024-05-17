int yr_arena_coalesce(
    YR_ARENA* arena)
{
  YR_ARENA_PAGE* page;
  YR_ARENA_PAGE* big_page;
  YR_ARENA_PAGE* next_page;
  YR_RELOC* reloc;

  uint8_t** reloc_address;
  uint8_t* reloc_target;
  size_t total_size = 0;

  page = arena->page_list_head;

  while(page != NULL)
  {
    total_size += page->used;
    page = page->next;
  }

  big_page = _yr_arena_new_page(total_size);

  if (big_page == NULL)
    return ERROR_INSUFFICIENT_MEMORY;

  page = arena->page_list_head;

  while (page != NULL)
  {
    page->new_address = big_page->address + big_page->used;
    memcpy(page->new_address, page->address, page->used);

    reloc = page->reloc_list_head;

    while(reloc != NULL)
    {
      reloc->offset += (uint32_t) big_page->used;
      reloc = reloc->next;
    }

    if (big_page->reloc_list_head == NULL)
      big_page->reloc_list_head = page->reloc_list_head;

    if (big_page->reloc_list_tail != NULL)
      big_page->reloc_list_tail->next = page->reloc_list_head;

    if (page->reloc_list_tail != NULL)
      big_page->reloc_list_tail = page->reloc_list_tail;

    big_page->used += page->used;
    page = page->next;
  }

  reloc = big_page->reloc_list_head;

  while (reloc != NULL)
  {
    reloc_address = (uint8_t**) (big_page->address + reloc->offset);
    reloc_target = *reloc_address;

    if (reloc_target != NULL)
    {
      page = _yr_arena_page_for_address(arena, reloc_target);
      assert(page != NULL);
      *reloc_address = page->new_address + (reloc_target - page->address);
    }

    reloc = reloc->next;
  }

  page = arena->page_list_head;

  while(page != NULL)
  {
    next_page = page->next;
    yr_free(page->address);
    yr_free(page);
    page = next_page;
  }

  arena->page_list_head = big_page;
  arena->current_page = big_page;
  arena->flags |= ARENA_FLAGS_COALESCED;

  return ERROR_SUCCESS;
}