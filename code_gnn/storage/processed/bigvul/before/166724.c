BasePage::BasePage(PageMemory* storage, BaseArena* arena)
    : magic_(GetMagic()),
      storage_(storage),
      arena_(arena),
      next_(nullptr),
      swept_(true) {
#if DCHECK_IS_ON()
  DCHECK(IsPageHeaderAddress(reinterpret_cast<Address>(this)));
#endif
}
