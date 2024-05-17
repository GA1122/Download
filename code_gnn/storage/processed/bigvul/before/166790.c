bool BaseArena::WillObjectBeLazilySwept(BasePage* page,
                                        void* object_pointer) const {
  if (page != first_unswept_page_)
    return true;

  DCHECK(!page->IsLargeObjectPage());
  NormalPage* normal_page = reinterpret_cast<NormalPage*>(page);
  NormalPageArena* normal_arena = normal_page->ArenaForNormalPage();
  if (!normal_arena->IsLazySweeping())
    return true;

  Address page_end = normal_page->PayloadEnd();
  for (Address header_address = normal_page->Payload();
       header_address < page_end;) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(header_address);
    size_t size = header->size();
    if (header_address > object_pointer)
      return false;
    if (!header->IsFree() && header->IsMarked()) {
      DCHECK(header_address + size < page_end);
      return true;
    }
    header_address += size;
  }
  NOTREACHED();
  return true;
}
