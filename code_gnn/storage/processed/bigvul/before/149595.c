void PreconnectManager::AllPreresolvesForUrlFinished(PreresolveInfo* info) {
  DCHECK(info);
  DCHECK(info->is_done());
  auto it = preresolve_info_.find(info->url.host());
  DCHECK(it != preresolve_info_.end());
  DCHECK(info == it->second.get());
  if (delegate_)
    delegate_->PreconnectFinished(std::move(info->stats));
  preresolve_info_.erase(it);
}
