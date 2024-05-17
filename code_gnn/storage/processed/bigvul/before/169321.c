void SetCookieCallback(bool* result, base::RunLoop* run_loop, bool success) {
  *result = success;
  run_loop->Quit();
}
