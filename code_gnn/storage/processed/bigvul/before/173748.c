static int handle_readdir(struct fuse* fuse, struct fuse_handler* handler,
 const struct fuse_in_header* hdr, const struct fuse_read_in* req)
{
 char buffer[8192];
 struct fuse_dirent *fde = (struct fuse_dirent*) buffer;
 struct dirent *de;
 struct dirhandle *h = id_to_ptr(req->fh);

    TRACE("[%d] READDIR %p\n", handler->token, h);
 if (req->offset == 0) {
  
        TRACE("[%d] calling rewinddir()\n", handler->token);
        rewinddir(h->d);
 }
    de = readdir(h->d);
 if (!de) {
 return 0;
 }
    fde->ino = FUSE_UNKNOWN_INO;
  
    fde->off = req->offset + 1;
    fde->type = de->d_type;
    fde->namelen = strlen(de->d_name);
    memcpy(fde->name, de->d_name, fde->namelen + 1);
    fuse_reply(fuse, hdr->unique, fde,
            FUSE_DIRENT_ALIGN(sizeof(struct fuse_dirent) + fde->namelen));
 return NO_STATUS;
}
