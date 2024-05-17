void OnGetFileInfoByResourceId(Profile* profile,
                               const std::string& resource_id,
                               base::PlatformFileError error,
                               const FilePath&  ,
                               scoped_ptr<GDataFileProto> file_proto) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (error != base::PLATFORM_FILE_OK)
    return;

  DCHECK(file_proto.get());
  const std::string& file_name = file_proto->gdata_entry().file_name();
  const GURL edit_url = GetFileResourceUrl(resource_id, file_name);
  OpenEditURLUIThread(profile, &edit_url);
  DVLOG(1) << "OnFindEntryByResourceId " << edit_url;
}
