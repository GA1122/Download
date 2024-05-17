void Compositor::DidGenerateLocalSurfaceIdAllocation(
    const viz::LocalSurfaceIdAllocation& allocation) {
  for (auto& observer : observer_list_)
    observer.DidGenerateLocalSurfaceIdAllocation(this, allocation);
}
