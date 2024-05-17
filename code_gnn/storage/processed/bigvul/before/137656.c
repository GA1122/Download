bool ShouldUseMojoChannel() {
  return CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kEnableRendererMojoChannel);
}
