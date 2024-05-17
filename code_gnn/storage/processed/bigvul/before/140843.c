void GLES2DecoderImpl::MaybeExitOnContextLost() {
  if (workarounds().exit_on_context_lost) {
    LOG(ERROR) << "Exiting GPU process because some drivers cannot reset"
               << " a D3D device in the Chrome GPU process sandbox.";
#if defined(OS_WIN)
    base::win::SetShouldCrashOnProcessDetach(false);
#endif
    exit(0);
  }
}
