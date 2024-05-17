static void ring_buffer_put(struct ring_buffer *rb)
{
	if (!atomic_dec_and_test(&rb->refcount))
		return;

	call_rcu(&rb->rcu_head, rb_free_rcu);
}
