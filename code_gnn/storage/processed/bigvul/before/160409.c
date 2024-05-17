bool BaseArena::willObjectBeLazilySwept(BasePage* page,
                                        void* objectPointer) const {
  if (page != m_firstUnsweptPage)
    return true;

  DCHECK(!page->isLargeObjectPage());
  NormalPage* normalPage = reinterpret_cast<NormalPage*>(page);
  NormalPageArena* normalArena = normalPage->arenaForNormalPage();
  if (!normalArena->isLazySweeping())
    return true;

  Address pageEnd = normalPage->payloadEnd();
  for (Address headerAddress = normalPage->payload();
       headerAddress < pageEnd;) {
    HeapObjectHeader* header =
        reinterpret_cast<HeapObjectHeader*>(headerAddress);
    size_t size = header->size();
    if (headerAddress > objectPointer)
      return false;
    if (!header->isFree() && header->isMarked()) {
      DCHECK(headerAddress + size < pageEnd);
      return true;
    }
    headerAddress += size;
  }
  NOTREACHED();
  return true;
}
