void ResizeWebContentsView(Shell* shell, const gfx::Size& size,
                           bool set_start_page) {
#if defined(TOOLKIT_GTK) || defined(OS_MACOSX)
  shell->SizeTo(size.width(), size.height());
  if (set_start_page)
    NavigateToURL(shell, GURL("about://blank"));
#else
  shell->web_contents()->GetView()->SizeContents(size);
#endif   
}
