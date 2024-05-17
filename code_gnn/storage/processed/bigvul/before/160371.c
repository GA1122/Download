void NormalPageArena::freePage(NormalPage* page) {
  getThreadState()->heap().heapStats().decreaseAllocatedSpace(page->size());

  PageMemory* memory = page->storage();
  page->~NormalPage();
  getThreadState()->heap().getFreePagePool()->add(arenaIndex(), memory);
}
