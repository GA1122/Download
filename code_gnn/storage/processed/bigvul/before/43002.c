static void virtnet_napi_enable(struct receive_queue *rq)
{
	napi_enable(&rq->napi);

	 
	if (napi_schedule_prep(&rq->napi)) {
		virtqueue_disable_cb(rq->vq);
		local_bh_disable();
		__napi_schedule(&rq->napi);
		local_bh_enable();
	}
}
