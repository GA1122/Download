static bool check_caller_access_to_name(struct fuse* fuse,
 const struct fuse_in_header *hdr, const struct node* parent_node,
 const char* name, int mode) {
  
 if (parent_node && parent_node->perm == PERM_ROOT) {
 if (!strcasecmp(name, "autorun.inf")
 || !strcasecmp(name, ".android_secure")
 || !strcasecmp(name, "android_secure")) {
 return false;
 }
 }

  
 if (hdr->uid == 0) {
 return true;
 }

  
 return true;
}
