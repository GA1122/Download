static void fuse_writepage_end(struct fuse_conn *fc, struct fuse_req *req)
{
	struct inode *inode = req->inode;
	struct fuse_inode *fi = get_fuse_inode(inode);

	mapping_set_error(inode->i_mapping, req->out.h.error);
	spin_lock(&fc->lock);
	while (req->misc.write.next) {
		struct fuse_conn *fc = get_fuse_conn(inode);
		struct fuse_write_in *inarg = &req->misc.write.in;
		struct fuse_req *next = req->misc.write.next;
		req->misc.write.next = next->misc.write.next;
		next->misc.write.next = NULL;
		next->ff = fuse_file_get(req->ff);
		list_add(&next->writepages_entry, &fi->writepages);

		 
		fuse_send_writepage(fc, next, inarg->offset + inarg->size);
	}
	fi->writectr--;
	fuse_writepage_finish(fc, req);
	spin_unlock(&fc->lock);
	fuse_writepage_free(fc, req);
}
