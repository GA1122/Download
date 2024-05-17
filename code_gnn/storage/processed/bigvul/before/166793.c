BaseArena::~BaseArena() {
  DCHECK(!first_page_);
  DCHECK(SweepingCompleted());
}
