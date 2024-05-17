void vfs_remove_all_fsp_extensions(files_struct *fsp)
{
	struct vfs_fsp_data *curr;
	struct vfs_fsp_data *next;

	for (curr = fsp->vfs_extension; curr; curr = next) {

		next = curr->next;
		fsp->vfs_extension = next;

		if (curr->destroy) {
			curr->destroy(EXT_DATA_AREA(curr));
		}
		TALLOC_FREE(curr);
	}
}
