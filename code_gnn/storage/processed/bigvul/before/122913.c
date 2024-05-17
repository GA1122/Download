void RenderProcessHost::SetRunRendererInProcess(bool value) {
  g_run_renderer_in_process_ = value;

  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (value && !command_line->HasSwitch(switches::kLang)) {
    const std::string locale =
        GetContentClient()->browser()->GetApplicationLocale();
    command_line->AppendSwitchASCII(switches::kLang, locale);
  }
}
