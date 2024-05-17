 void DownloadFileManager::CompleteDownload(DownloadId global_id) {
// void DownloadFileManager::CompleteDownload(
//     DownloadId global_id, const base::Closure& callback) {
    DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  
    if (!ContainsKey(downloads_, global_id))
     return;
 
   DownloadFile* download_file = downloads_[global_id];
 
   VLOG(20) << " " << __FUNCTION__ << "()"
             << " id = " << global_id
             << " download_file = " << download_file->DebugString();
  
//    
//    
//    
//    
//    
//    
//   download_file->AnnotateWithSourceInformation();
// 
    download_file->Detach();
  
    EraseDownload(global_id);
// 
//    
//   BrowserThread::PostTask(BrowserThread::UI, FROM_HERE, callback);
  }