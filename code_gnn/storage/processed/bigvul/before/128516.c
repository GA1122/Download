ShellSurface::ShellSurface(Surface* surface)
    : ShellSurface(surface,
                   nullptr,
                   gfx::Rect(),
                   true,
                   ash::kShellWindowId_DefaultContainer) {}
