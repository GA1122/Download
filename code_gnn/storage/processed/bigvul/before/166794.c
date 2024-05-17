NormalPage::~NormalPage() {
#if DCHECK_IS_ON()
  DCHECK(IsPageHeaderAddress(reinterpret_cast<Address>(this)));
#endif
}
