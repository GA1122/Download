static void skb_xmit_done(struct virtqueue *vq)
{
	struct virtnet_info *vi = vq->vdev->priv;

	 
	virtqueue_disable_cb(vq);

	 
	netif_wake_subqueue(vi->dev, vq2txq(vq));
}
