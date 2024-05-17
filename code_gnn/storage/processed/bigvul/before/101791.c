void Browser::RunFileChooserHelper(
    TabContents* tab, const ViewHostMsg_RunFileChooser_Params& params) {
  Profile* profile =
      Profile::FromBrowserContext(tab->browser_context());

  FileSelectHelper* file_select_helper = new FileSelectHelper(profile);

  file_select_helper->RunFileChooser(tab->render_view_host(), tab, params);
}
