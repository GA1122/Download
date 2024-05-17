AshWindowTreeHostInitParams ShellInitParamsToAshWindowTreeHostInitParams(
    const ShellInitParams& shell_init_params) {
  AshWindowTreeHostInitParams ash_init_params;
#if defined(OS_WIN)
  ash_init_params.remote_hwnd = shell_init_params.remote_hwnd;
#endif
  return ash_init_params;
}
