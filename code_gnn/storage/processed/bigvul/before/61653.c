heap_add_entry(struct archive_read *a,
    struct heap_queue *heap, struct xar_file *file)
{
	uint64_t file_id, parent_id;
	int hole, parent;

	 
	if (heap->used >= heap->allocated) {
		struct xar_file **new_pending_files;
		int new_size = heap->allocated * 2;

		if (heap->allocated < 1024)
			new_size = 1024;
		 
		if (new_size <= heap->allocated) {
			archive_set_error(&a->archive,
			    ENOMEM, "Out of memory");
			return (ARCHIVE_FATAL);
		}
		new_pending_files = (struct xar_file **)
		    malloc(new_size * sizeof(new_pending_files[0]));
		if (new_pending_files == NULL) {
			archive_set_error(&a->archive,
			    ENOMEM, "Out of memory");
			return (ARCHIVE_FATAL);
		}
		memcpy(new_pending_files, heap->files,
		    heap->allocated * sizeof(new_pending_files[0]));
		if (heap->files != NULL)
			free(heap->files);
		heap->files = new_pending_files;
		heap->allocated = new_size;
	}

	file_id = file->id;

	 
	hole = heap->used++;
	while (hole > 0) {
		parent = (hole - 1)/2;
		parent_id = heap->files[parent]->id;
		if (file_id >= parent_id) {
			heap->files[hole] = file;
			return (ARCHIVE_OK);
		}
		 
		heap->files[hole] = heap->files[parent];
		hole = parent;
	}
	heap->files[0] = file;

	return (ARCHIVE_OK);
}
