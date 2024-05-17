void NormalPageArena::SetAllocationPoint(Address point, size_t size) {
#if DCHECK_IS_ON()
  if (point) {
    DCHECK(size);
    BasePage* page = PageFromObject(point);
    DCHECK(!page->IsLargeObjectPage());
    DCHECK_LE(size, static_cast<NormalPage*>(page)->PayloadSize());
  }
#endif
  if (HasCurrentAllocationArea()) {
    AddToFreeList(CurrentAllocationPoint(), RemainingAllocationSize());
  }
  UpdateRemainingAllocationSize();
  current_allocation_point_ = point;
  last_remaining_allocation_size_ = remaining_allocation_size_ = size;
  if (point) {
    NormalPage* page = reinterpret_cast<NormalPage*>(PageFromObject(point));
    page->object_start_bit_map()->ClearBit(point);
  }
}
