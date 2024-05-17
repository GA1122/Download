int yr_arena_duplicate(
    YR_ARENA* arena,
    YR_ARENA** duplicated)
{
  YR_RELOC* reloc;
  YR_RELOC* new_reloc;
  YR_ARENA_PAGE* page;
  YR_ARENA_PAGE* new_page;
  YR_ARENA* new_arena;
  uint8_t** reloc_address;
  uint8_t* reloc_target;

  assert(arena->flags & ARENA_FLAGS_COALESCED);

  page = arena->page_list_head;

  FAIL_ON_ERROR(yr_arena_create(page->size, arena->flags, &new_arena));

  new_page = new_arena->current_page;
  new_page->used = page->used;

  memcpy(new_page->address, page->address, page->size);

  reloc = page->reloc_list_head;

  while (reloc != NULL)
  {
    new_reloc = (YR_RELOC*) yr_malloc(sizeof(YR_RELOC));

    if (new_reloc == NULL)
    {
      yr_arena_destroy(new_arena);
      return ERROR_INSUFFICIENT_MEMORY;
    }

    new_reloc->offset = reloc->offset;
    new_reloc->next = NULL;

    if (new_page->reloc_list_head == NULL)
      new_page->reloc_list_head = new_reloc;

    if (new_page->reloc_list_tail != NULL)
      new_page->reloc_list_tail->next = new_reloc;

    new_page->reloc_list_tail = new_reloc;

    reloc_address = (uint8_t**) (new_page->address + new_reloc->offset);
    reloc_target = *reloc_address;

    if (reloc_target != NULL)
    {
      assert(reloc_target >= page->address);
      assert(reloc_target < page->address + page->used);

      *reloc_address = reloc_target - \
                       page->address + \
                       new_page->address;
    }

    reloc = reloc->next;
  }

  *duplicated = new_arena;

  return ERROR_SUCCESS;
}