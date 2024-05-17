TrayCast::~TrayCast() {
  Shell::GetInstance()->RemoveShellObserver(this);
}
