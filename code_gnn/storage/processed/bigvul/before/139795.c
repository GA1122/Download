void Shell::AddShellObserver(ShellObserver* observer) {
  observers_.AddObserver(observer);
}
