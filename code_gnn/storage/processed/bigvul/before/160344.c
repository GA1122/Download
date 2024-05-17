NormalPage::NormalPage(PageMemory* storage, BaseArena* arena)
    : BasePage(storage, arena), m_objectStartBitMapComputed(false) {
  ASSERT(isPageHeaderAddress(reinterpret_cast<Address>(this)));
}
