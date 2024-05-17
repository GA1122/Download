viz::LocalSurfaceIdAllocation Compositor::RequestNewChildLocalSurfaceId() {
  const uint32_t child_sequence_number =
      host_->GenerateChildSurfaceSequenceNumberSync();
  const viz::LocalSurfaceId current_id =
      host_->local_surface_id_allocation_from_parent().local_surface_id();
  return viz::LocalSurfaceIdAllocation(
      viz::LocalSurfaceId(current_id.parent_sequence_number(),
                          child_sequence_number, current_id.embed_token()),
      base::TimeTicks::Now());
}
