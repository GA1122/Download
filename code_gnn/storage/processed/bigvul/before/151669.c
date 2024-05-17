void Browser::EnumerateDirectory(WebContents* web_contents,
                                 int request_id,
                                 const base::FilePath& path) {
  FileSelectHelper::EnumerateDirectory(web_contents, request_id, path);
}
