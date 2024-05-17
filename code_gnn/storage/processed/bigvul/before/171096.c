void dump_camera_metadata(const camera_metadata_t *metadata,
 int fd,
 int verbosity) {
    dump_indented_camera_metadata(metadata, fd, verbosity, 0);
}
