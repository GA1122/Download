GDataFileSystem::GDataFileSystem(
    Profile* profile,
    GDataCache* cache,
    DocumentsServiceInterface* documents_service,
    GDataUploaderInterface* uploader,
    DriveWebAppsRegistryInterface* webapps_registry,
    base::SequencedTaskRunner* blocking_task_runner)
    : profile_(profile),
      cache_(cache),
      uploader_(uploader),
      documents_service_(documents_service),
      webapps_registry_(webapps_registry),
      update_timer_(true  , true  ),
      hide_hosted_docs_(false),
      blocking_task_runner_(blocking_task_runner),
      ui_weak_ptr_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      ui_weak_ptr_(ui_weak_ptr_factory_.GetWeakPtr()) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
}
