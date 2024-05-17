bool EnableInProcessStackDumpingForSandbox() {
#if defined(USE_SYMBOLIZE)
  SandboxSymbolizeHelper::GetInstance();
#endif   

  return EnableInProcessStackDumping();
}
