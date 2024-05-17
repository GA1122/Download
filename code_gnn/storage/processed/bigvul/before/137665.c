void PrintPreviewDialogDelegate::GetDialogSize(gfx::Size* size) const {
  DCHECK(size);
  const gfx::Size kMinDialogSize(800, 480);
  const int kBorder = 25;
  *size = kMinDialogSize;

  web_modal::WebContentsModalDialogHost* host = nullptr;
  content::WebContents* outermost_web_contents =
      guest_view::GuestViewBase::GetTopLevelWebContents(initiator_);
  Browser* browser = chrome::FindBrowserWithWebContents(outermost_web_contents);
  if (browser)
    host = browser->window()->GetWebContentsModalDialogHost();

  if (host)
    size->SetToMax(host->GetMaximumDialogSize());
  else
    size->SetToMax(outermost_web_contents->GetContainerBounds().size());
  size->Enlarge(-2 * kBorder, -kBorder);

#if defined(OS_MACOSX)
  const gfx::Size kMaxDialogSize(1000, 660);
  size->SetToMin(kMaxDialogSize);
#endif
}
