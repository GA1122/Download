void ChromeDownloadManagerDelegate::OpenWithWebIntent(
    const DownloadItem* item) {
  webkit_glue::WebIntentData intent_data(
      ASCIIToUTF16("http://webintents.org/view"),
      ASCIIToUTF16(item->GetMimeType()),
      item->GetFullPath(),
      item->GetReceivedBytes());

  intent_data.extra_data.insert(make_pair(
      ASCIIToUTF16("url"), ASCIIToUTF16(item->GetURL().spec())));

  string16 filename = UTF8ToUTF16(item->GetSuggestedFilename());
  if (filename.empty())
    filename = item->GetFileNameToReportUser().LossyDisplayName();
  intent_data.extra_data.insert(make_pair(ASCIIToUTF16("filename"), filename));

  content::WebIntentsDispatcher* dispatcher =
      content::WebIntentsDispatcher::Create(intent_data);
  item->GetWebContents()->GetDelegate()->WebIntentDispatch(
      item->GetWebContents(), dispatcher);
}
