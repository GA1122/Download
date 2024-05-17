void UrlData::LoadNow() {
  std::vector<base::OnceClosure> waiting_load_callbacks;
  std::swap(waiting_load_callbacks, waiting_load_callbacks_);
  for (auto& i : waiting_load_callbacks)
    std::move(i).Run();
}
