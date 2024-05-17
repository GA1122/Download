static int handle_fuse_request(struct fuse *fuse, struct fuse_handler* handler,
 const struct fuse_in_header *hdr, const void *data, size_t data_len)
{
 switch (hdr->opcode) {
 case FUSE_LOOKUP: {  
 const char* name = data;
 return handle_lookup(fuse, handler, hdr, name);
 }

 case FUSE_FORGET: {
 const struct fuse_forget_in *req = data;
 return handle_forget(fuse, handler, hdr, req);
 }

 case FUSE_GETATTR: {  
 const struct fuse_getattr_in *req = data;
 return handle_getattr(fuse, handler, hdr, req);
 }

 case FUSE_SETATTR: {  
 const struct fuse_setattr_in *req = data;
 return handle_setattr(fuse, handler, hdr, req);
 }

 case FUSE_MKNOD: {  
 const struct fuse_mknod_in *req = data;
 const char *name = ((const char*) data) + sizeof(*req);
 return handle_mknod(fuse, handler, hdr, req, name);
 }

 case FUSE_MKDIR: {  
 const struct fuse_mkdir_in *req = data;
 const char *name = ((const char*) data) + sizeof(*req);
 return handle_mkdir(fuse, handler, hdr, req, name);
 }

 case FUSE_UNLINK: {  
 const char* name = data;
 return handle_unlink(fuse, handler, hdr, name);
 }

 case FUSE_RMDIR: {  
 const char* name = data;
 return handle_rmdir(fuse, handler, hdr, name);
 }

 case FUSE_RENAME: {  
 const struct fuse_rename_in *req = data;
 const char *old_name = ((const char*) data) + sizeof(*req);
 const char *new_name = old_name + strlen(old_name) + 1;
 return handle_rename(fuse, handler, hdr, req, old_name, new_name);
 }

 case FUSE_OPEN: {  
 const struct fuse_open_in *req = data;
 return handle_open(fuse, handler, hdr, req);
 }

 case FUSE_READ: {  
 const struct fuse_read_in *req = data;
 return handle_read(fuse, handler, hdr, req);
 }

 case FUSE_WRITE: {  
 const struct fuse_write_in *req = data;
 const void* buffer = (const __u8*)data + sizeof(*req);
 return handle_write(fuse, handler, hdr, req, buffer);
 }

 case FUSE_STATFS: {  
 return handle_statfs(fuse, handler, hdr);
 }

 case FUSE_RELEASE: {  
 const struct fuse_release_in *req = data;
 return handle_release(fuse, handler, hdr, req);
 }

 case FUSE_FSYNC:
 case FUSE_FSYNCDIR: {
 const struct fuse_fsync_in *req = data;
 return handle_fsync(fuse, handler, hdr, req);
 }

 case FUSE_FLUSH: {
 return handle_flush(fuse, handler, hdr);
 }

 case FUSE_OPENDIR: {  
 const struct fuse_open_in *req = data;
 return handle_opendir(fuse, handler, hdr, req);
 }

 case FUSE_READDIR: {
 const struct fuse_read_in *req = data;
 return handle_readdir(fuse, handler, hdr, req);
 }

 case FUSE_RELEASEDIR: {  
 const struct fuse_release_in *req = data;
 return handle_releasedir(fuse, handler, hdr, req);
 }

 case FUSE_INIT: {  
 const struct fuse_init_in *req = data;
 return handle_init(fuse, handler, hdr, req);
 }

 default: {
        TRACE("[%d] NOTIMPL op=%d uniq=%"PRIx64" nid=%"PRIx64"\n",
                handler->token, hdr->opcode, hdr->unique, hdr->nodeid);
 return -ENOSYS;
 }
 }
}