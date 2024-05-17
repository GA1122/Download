static bool ExecutePrint(LocalFrame& frame,
                         Event*,
                         EditorCommandSource,
                         const String&) {
  Page* page = frame.GetPage();
  if (!page)
    return false;
  return page->GetChromeClient().Print(&frame);
}
