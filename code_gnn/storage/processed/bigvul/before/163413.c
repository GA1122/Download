bool RenderThreadImpl::ShouldBeDestroyed() {
  DCHECK(base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kSingleProcess));
  base::AtExitManager::DisableAllAtExitManagers();
  return false;
}
