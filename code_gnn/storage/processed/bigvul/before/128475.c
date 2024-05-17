void ShellSurface::Close() {
  if (!close_callback_.is_null())
    close_callback_.Run();
}
