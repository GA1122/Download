base::RepeatingCallback<void(Args...)> CreateSafeCallback(
    SkiaOutputSurfaceDependency* dependency,
    const base::RepeatingCallback<void(Args...)>& callback) {
  DCHECK(dependency);
  return base::BindRepeating(&PostAsyncTask<Args...>, dependency, callback);
}
