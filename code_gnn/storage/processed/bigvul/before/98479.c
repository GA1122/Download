bool AeroPeekManager::GetTabThumbnail(int tab_id, SkBitmap* thumbnail) {
  DCHECK(thumbnail);

  TabContents* contents = GetTabContents(tab_id);
  if (!contents)
    return false;

  ThumbnailGenerator* generator = g_browser_process->GetThumbnailGenerator();
  DCHECK(generator);
  *thumbnail = generator->GetThumbnailForRenderer(contents->render_view_host());

  return true;
}
