void UrlData::WaitToLoad(base::OnceClosure cb) {
  if (!IsPreloading()) {
    std::move(cb).Run();
  } else {
    waiting_load_callbacks_.emplace_back(std::move(cb));
    if (waiting_load_callbacks_.size() == 1)
      url_index_->WaitToLoad(this);
  }
}
