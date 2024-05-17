int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);   
  CommandLine::Init(argc, argv);

  CommandLine* cmd_line = CommandLine::ForCurrentProcess();
  DCHECK(cmd_line);

  CommandLine::SwitchMap switches = cmd_line->GetSwitches();
  for (CommandLine::SwitchMap::const_iterator it = switches.begin();
       it != switches.end(); ++it) {
    if (it->first == "test_video_data") {
      test_video_data = it->second.c_str();
      continue;
    }
    LOG(FATAL) << "Unexpected switch: " << it->first << ":" << it->second;
  }
#if defined(OS_WIN)
  base::ShadowingAtExitManager at_exit_manager;
  gfx::InitializeGLBindings(gfx::kGLImplementationEGLGLES2);
  gfx::GLSurface::InitializeOneOff();
  {
    scoped_refptr<gfx::GLSurface> surface(
        gfx::GLSurface::CreateOffscreenGLSurface(false, gfx::Size(1, 1)));
    scoped_refptr<gfx::GLContext> context(
        gfx::GLContext::CreateGLContext(NULL, surface.get(),
                                        gfx::PreferIntegratedGpu));
    context->MakeCurrent(surface.get());
  }
  DXVAVideoDecodeAccelerator::PreSandboxInitialization();
#endif   
  return RUN_ALL_TESTS();
}
