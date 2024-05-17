void RenderWidgetHostImpl::OnSnapshotDataReceivedAsync(
    int snapshot_id,
    scoped_refptr<base::RefCountedBytes> png_data) {
  if (png_data.get())
    OnSnapshotDataReceived(snapshot_id, png_data->front(), png_data->size());
  else
    OnSnapshotDataReceived(snapshot_id, NULL, 0);
}
