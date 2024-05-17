void BrowserRenderProcessHost::ReceivedBadMessage() {
  if (run_renderer_in_process()) {
    CHECK(false);
  }
  NOTREACHED();
  base::KillProcess(GetHandle(), content::RESULT_CODE_KILLED_BAD_MESSAGE,
                    false);
}
