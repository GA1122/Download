void xen_blkbk_free_caches(struct xen_blkif_ring *ring)
{
	 
	if (!RB_EMPTY_ROOT(&ring->persistent_gnts))
		free_persistent_gnts(ring, &ring->persistent_gnts,
			ring->persistent_gnt_c);

	BUG_ON(!RB_EMPTY_ROOT(&ring->persistent_gnts));
	ring->persistent_gnt_c = 0;

	 
	shrink_free_pagepool(ring, 0  );
}
