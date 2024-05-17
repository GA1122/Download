void PrepareDragData(const DropData& drop_data,
                     ui::OSExchangeData::Provider* provider,
                     WebContentsImpl* web_contents) {
#if defined(OS_WIN)
  if (!drop_data.download_metadata.empty())
    PrepareDragForDownload(drop_data, provider, web_contents);
  if (!drop_data.file_contents.empty())
    PrepareDragForFileContents(drop_data, provider);
#endif
  if (!drop_data.text.string().empty())
    provider->SetString(drop_data.text.string());
  if (drop_data.url.is_valid())
    provider->SetURL(drop_data.url, drop_data.url_title);
  if (!drop_data.html.string().empty())
    provider->SetHtml(drop_data.html.string(), drop_data.html_base_url);
  if (!drop_data.filenames.empty()) {
    std::vector<ui::OSExchangeData::FileInfo> filenames;
    for (std::vector<DropData::FileInfo>::const_iterator it =
             drop_data.filenames.begin();
         it != drop_data.filenames.end(); ++it) {
      filenames.push_back(
          ui::OSExchangeData::FileInfo(
              base::FilePath::FromUTF8Unsafe(base::UTF16ToUTF8(it->path)),
              base::FilePath::FromUTF8Unsafe(
                  base::UTF16ToUTF8(it->display_name))));
    }
    provider->SetFilenames(filenames);
  }
  if (!drop_data.custom_data.empty()) {
    Pickle pickle;
    ui::WriteCustomDataToPickle(drop_data.custom_data, &pickle);
    provider->SetPickledData(ui::Clipboard::GetWebCustomDataFormatType(),
                             pickle);
  }
}
