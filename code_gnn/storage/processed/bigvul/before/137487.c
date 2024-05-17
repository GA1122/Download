void RunLoop::QuitCurrentWhenIdleDeprecated() {
  DCHECK(IsRunningOnCurrentThread());
  tls_delegate.Get().Get()->active_run_loops_.top()->QuitWhenIdle();
}
