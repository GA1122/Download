viz::LocalSurfaceIdAllocation Compositor::UpdateLocalSurfaceIdFromParent(
    const viz::LocalSurfaceIdAllocation& local_surface_id_allocation) {
  DCHECK(local_surface_id_allocation.IsValid());
  if (!host_->local_surface_id_allocation_from_parent().IsValid()) {
    host_->SetLocalSurfaceIdAllocationFromParent(local_surface_id_allocation);
    return local_surface_id_allocation;
  }
  const viz::LocalSurfaceId& current_id =
      host_->local_surface_id_allocation_from_parent().local_surface_id();
  auto allocator =
      viz::ChildLocalSurfaceIdAllocator::CreateWithChildSequenceNumber(
          current_id.child_sequence_number());
  allocator->UpdateFromParent(local_surface_id_allocation);
  const viz::LocalSurfaceIdAllocation resulting_id =
      allocator->GetCurrentLocalSurfaceIdAllocation();
  host_->SetLocalSurfaceIdAllocationFromParent(resulting_id);
  return resulting_id;
}
