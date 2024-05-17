int yr_arena_load_stream(
    YR_STREAM* stream,
    YR_ARENA** arena)
{
  YR_ARENA_PAGE* page;
  YR_ARENA* new_arena;
  ARENA_FILE_HEADER header;

  uint32_t reloc_offset;
  uint8_t** reloc_address;
  uint8_t* reloc_target;

  int result;

  if (yr_stream_read(&header, sizeof(header), 1, stream) != 1)
    return ERROR_INVALID_FILE;

  if (header.magic[0] != 'Y' ||
      header.magic[1] != 'A' ||
      header.magic[2] != 'R' ||
      header.magic[3] != 'A')
  {
    return ERROR_INVALID_FILE;
  }

  if (header.size < 2048)        
    return ERROR_CORRUPT_FILE;

  if (header.version != ARENA_FILE_VERSION)
    return ERROR_UNSUPPORTED_FILE_VERSION;

  result = yr_arena_create(header.size, 0, &new_arena);

  if (result != ERROR_SUCCESS)
    return result;

  page = new_arena->current_page;

  if (yr_stream_read(page->address, header.size, 1, stream) != 1)
  {
    yr_arena_destroy(new_arena);
    return ERROR_CORRUPT_FILE;
  }

  page->used = header.size;

  if (yr_stream_read(&reloc_offset, sizeof(reloc_offset), 1, stream) != 1)
  {
    yr_arena_destroy(new_arena);
    return ERROR_CORRUPT_FILE;
  }

  while (reloc_offset != 0xFFFFFFFF)
  {
    if (reloc_offset > header.size - sizeof(uint8_t*))
    {
      yr_arena_destroy(new_arena);
      return ERROR_CORRUPT_FILE;
    }

    yr_arena_make_relocatable(new_arena, page->address, reloc_offset, EOL);

    reloc_address = (uint8_t**) (page->address + reloc_offset);
    reloc_target = *reloc_address;

    if (reloc_target != (uint8_t*) (size_t) 0xFFFABADA)
      *reloc_address += (size_t) page->address;
    else
      *reloc_address = 0;

    if (yr_stream_read(&reloc_offset, sizeof(reloc_offset), 1, stream) != 1)
    {
      yr_arena_destroy(new_arena);
      return ERROR_CORRUPT_FILE;
    }
  }

  *arena = new_arena;

  return ERROR_SUCCESS;
}