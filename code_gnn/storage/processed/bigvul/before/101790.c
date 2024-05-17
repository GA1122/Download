void Browser::RunFileChooser(TabContents* tab,
                             const ViewHostMsg_RunFileChooser_Params& params) {
  RunFileChooserHelper(tab, params);
}
