static void xen_blkbk_unmap_and_respond_callback(int result, struct gntab_unmap_queue_data *data)
{
	struct pending_req *pending_req = (struct pending_req *)(data->data);
	struct xen_blkif_ring *ring = pending_req->ring;
	struct xen_blkif *blkif = ring->blkif;

	 
	BUG_ON(result);

	put_free_pages(ring, data->pages, data->count);
	make_response(ring, pending_req->id,
		      pending_req->operation, pending_req->status);
	free_req(ring, pending_req);
	 
	if (atomic_dec_and_test(&ring->inflight) && atomic_read(&blkif->drain)) {
		complete(&blkif->drain_complete);
	}
	xen_blkif_put(blkif);
}
