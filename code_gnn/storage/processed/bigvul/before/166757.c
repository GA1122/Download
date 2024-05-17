NormalPage::NormalPage(PageMemory* storage, BaseArena* arena)
    : BasePage(storage, arena), object_start_bit_map_(Payload()) {
#if DCHECK_IS_ON()
  DCHECK(IsPageHeaderAddress(reinterpret_cast<Address>(this)));
#endif   
}
