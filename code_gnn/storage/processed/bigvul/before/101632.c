void Browser::EnumerateDirectoryHelper(TabContents* tab, int request_id,
                                       const FilePath& path) {
  ChildProcessSecurityPolicy* policy =
      ChildProcessSecurityPolicy::GetInstance();
  if (!policy->CanReadDirectory(tab->render_view_host()->process()->id(),
                                path)) {
    return;
  }

  Profile* profile =
      Profile::FromBrowserContext(tab->browser_context());

  FileSelectHelper* file_select_helper = new FileSelectHelper(profile);

  file_select_helper->EnumerateDirectory(request_id,
                                         tab->render_view_host(),
                                         path);
}
