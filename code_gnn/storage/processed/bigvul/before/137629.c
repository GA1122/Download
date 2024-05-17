void MockDownloadController::AcquireFileAccessPermission(
    content::WebContents* web_contents,
    const DownloadControllerBase::AcquireFileAccessPermissionCallback& cb) {
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::Bind(cb, approve_file_access_request_));
}
