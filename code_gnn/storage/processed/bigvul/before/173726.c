static void derive_permissions_locked(struct fuse* fuse, struct node *parent,
 struct node *node) {
 appid_t appid;

  
    node->perm = PERM_INHERIT;
    node->userid = parent->userid;
    node->uid = parent->uid;
    node->under_android = parent->under_android;

  
 switch (parent->perm) {
 case PERM_INHERIT:
  
 break;
 case PERM_PRE_ROOT:
  
        node->perm = PERM_ROOT;
        node->userid = strtoul(node->name, NULL, 10);
 break;
 case PERM_ROOT:
  
 if (!strcasecmp(node->name, "Android")) {
  
            node->perm = PERM_ANDROID;
            node->under_android = true;
 }
 break;
 case PERM_ANDROID:
 if (!strcasecmp(node->name, "data")) {
  
            node->perm = PERM_ANDROID_DATA;
 } else if (!strcasecmp(node->name, "obb")) {
  
            node->perm = PERM_ANDROID_OBB;
  
            node->graft_path = fuse->global->obb_path;
            node->graft_pathlen = strlen(fuse->global->obb_path);
 } else if (!strcasecmp(node->name, "media")) {
  
            node->perm = PERM_ANDROID_MEDIA;
 }
 break;
 case PERM_ANDROID_DATA:
 case PERM_ANDROID_OBB:
 case PERM_ANDROID_MEDIA:
        appid = (appid_t) (uintptr_t) hashmapGet(fuse->global->package_to_appid, node->name);
 if (appid != 0) {
            node->uid = multiuser_get_uid(parent->userid, appid);
 }
 break;
 }
}
