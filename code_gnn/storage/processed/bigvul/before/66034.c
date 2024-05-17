int yr_arena_save_stream(
  YR_ARENA* arena,
  YR_STREAM* stream)
{
  YR_ARENA_PAGE* page;
  YR_RELOC* reloc;
  ARENA_FILE_HEADER header;

  uint32_t end_marker = 0xFFFFFFFF;
  uint8_t** reloc_address;
  uint8_t* reloc_target;

  assert(arena->flags & ARENA_FLAGS_COALESCED);

  page = arena->page_list_head;
  reloc = page->reloc_list_head;

  while (reloc != NULL)
  {
    reloc_address = (uint8_t**) (page->address + reloc->offset);
    reloc_target = *reloc_address;

    if (reloc_target != NULL)
    {
      assert(reloc_target >= page->address);
      assert(reloc_target < page->address + page->used);
      *reloc_address = (uint8_t*) (*reloc_address - page->address);
    }
    else
    {
      *reloc_address = (uint8_t*) (size_t) 0xFFFABADA;
    }

    reloc = reloc->next;
  }

  assert(page->size < 0x80000000);   

  header.magic[0] = 'Y';
  header.magic[1] = 'A';
  header.magic[2] = 'R';
  header.magic[3] = 'A';
  header.size = (int32_t) page->size;
  header.version = ARENA_FILE_VERSION;

  yr_stream_write(&header, sizeof(header), 1, stream);
  yr_stream_write(page->address, header.size, 1, stream);

  reloc = page->reloc_list_head;

  while (reloc != NULL)
  {
    yr_stream_write(&reloc->offset, sizeof(reloc->offset), 1, stream);

    reloc_address = (uint8_t**) (page->address + reloc->offset);
    reloc_target = *reloc_address;

    if (reloc_target != (void*) (size_t) 0xFFFABADA)
      *reloc_address += (size_t) page->address;
    else
      *reloc_address = 0;

    reloc = reloc->next;
  }

  yr_stream_write(&end_marker, sizeof(end_marker), 1, stream);

  return ERROR_SUCCESS;
}