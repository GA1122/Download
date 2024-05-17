void NormalPageArena::FreePage(NormalPage* page) {
  GetThreadState()->Heap().HeapStats().DecreaseAllocatedSpace(page->size());

  PageMemory* memory = page->Storage();
  page->~NormalPage();
  GetThreadState()->Heap().GetFreePagePool()->Add(ArenaIndex(), memory);
}
