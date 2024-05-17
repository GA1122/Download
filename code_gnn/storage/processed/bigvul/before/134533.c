void PrepareDragForDownload(
    const DropData& drop_data,
    ui::OSExchangeData::Provider* provider,
    WebContentsImpl* web_contents) {
  const GURL& page_url = web_contents->GetLastCommittedURL();
  const std::string& page_encoding = web_contents->GetEncoding();

  base::string16 mime_type;
  base::FilePath file_name;
  GURL download_url;
  if (!ParseDownloadMetadata(drop_data.download_metadata,
                             &mime_type,
                             &file_name,
                             &download_url))
    return;

  std::string default_name =
      GetContentClient()->browser()->GetDefaultDownloadName();
  base::FilePath generated_download_file_name =
      net::GenerateFileName(download_url,
                            std::string(),
                            std::string(),
                            base::UTF16ToUTF8(file_name.value()),
                            base::UTF16ToUTF8(mime_type),
                            default_name);

  base::ThreadRestrictions::ScopedAllowIO allow_file_operations;

  base::FilePath temp_dir_path;
  if (!base::CreateNewTempDirectory(FILE_PATH_LITERAL("chrome_drag"),
                                    &temp_dir_path))
    return;

  base::FilePath download_path =
      temp_dir_path.Append(generated_download_file_name);

  base::DeleteFileAfterReboot(download_path);
  base::DeleteFileAfterReboot(temp_dir_path);

  scoped_refptr<DragDownloadFile> download_file =
      new DragDownloadFile(
          download_path,
          scoped_ptr<net::FileStream>(),
          download_url,
          Referrer(page_url, drop_data.referrer_policy),
          page_encoding,
          web_contents);
  ui::OSExchangeData::DownloadFileInfo file_download(base::FilePath(),
                                                     download_file.get());
  provider->SetDownloadFileInfo(file_download);
}
