void ID3::Iterator::getString(String8 *id, String8 *comment) const {
    getstring(id, false);
 if (comment != NULL) {
        getstring(comment, true);
 }
}
