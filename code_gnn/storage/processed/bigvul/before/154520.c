void RunCallbacks(std::vector<base::OnceClosure> callbacks) {
  for (base::OnceClosure& callback : callbacks) {
    std::move(callback).Run();
  }
}
