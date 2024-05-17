static void attr_from_stat(struct fuse* fuse, struct fuse_attr *attr,
 const struct stat *s, const struct node* node) {
    attr->ino = node->ino;
    attr->size = s->st_size;
    attr->blocks = s->st_blocks;
    attr->atime = s->st_atim.tv_sec;
    attr->mtime = s->st_mtim.tv_sec;
    attr->ctime = s->st_ctim.tv_sec;
    attr->atimensec = s->st_atim.tv_nsec;
    attr->mtimensec = s->st_mtim.tv_nsec;
    attr->ctimensec = s->st_ctim.tv_nsec;
    attr->mode = s->st_mode;
    attr->nlink = s->st_nlink;

    attr->uid = node->uid;

 if (fuse->gid == AID_SDCARD_RW) {
  
        attr->gid = AID_SDCARD_RW;
 } else {
        attr->gid = multiuser_get_uid(node->userid, fuse->gid);
 }

 int visible_mode = 0775 & ~fuse->mask;
 if (node->perm == PERM_PRE_ROOT) {
  
        visible_mode = 0711;
 } else if (node->under_android) {
  
 if (fuse->gid == AID_SDCARD_RW) {
            visible_mode = visible_mode & ~0006;
 } else {
            visible_mode = visible_mode & ~0007;
 }
 }
 int owner_mode = s->st_mode & 0700;
 int filtered_mode = visible_mode & (owner_mode | (owner_mode >> 3) | (owner_mode >> 6));
    attr->mode = (attr->mode & S_IFMT) | filtered_mode;
}
