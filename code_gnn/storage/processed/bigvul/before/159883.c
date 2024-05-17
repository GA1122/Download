void TopSitesImpl::Init(const base::FilePath& db_name) {
  backend_ = new TopSitesBackend();
  backend_->Init(db_name);
  backend_->GetMostVisitedThumbnails(
      base::Bind(&TopSitesImpl::OnGotMostVisitedThumbnails,
                 base::Unretained(this)),
      &cancelable_task_tracker_);
}
