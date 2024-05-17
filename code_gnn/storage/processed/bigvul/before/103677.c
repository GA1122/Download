void BrowserRenderProcessHost::OnRevealFolderInOS(const FilePath& path) {
  if (ChildProcessSecurityPolicy::GetInstance()->CanReadFile(id(), path))
    content::GetContentClient()->browser()->RevealFolderInOS(path);
}
