heap_get_entry(struct heap_queue *heap)
{
	uint64_t a_id, b_id, c_id;
	int a, b, c;
	struct xar_file *r, *tmp;

	if (heap->used < 1)
		return (NULL);

	 
	r = heap->files[0];

	 
	heap->files[0] = heap->files[--(heap->used)];

	 
	a = 0;  
	a_id = heap->files[a]->id;
	for (;;) {
		b = a + a + 1;  
		if (b >= heap->used)
			return (r);
		b_id = heap->files[b]->id;
		c = b + 1;  
		if (c < heap->used) {
			c_id = heap->files[c]->id;
			if (c_id < b_id) {
				b = c;
				b_id = c_id;
			}
		}
		if (a_id <= b_id)
			return (r);
		tmp = heap->files[a];
		heap->files[a] = heap->files[b];
		heap->files[b] = tmp;
		a = b;
	}
}
