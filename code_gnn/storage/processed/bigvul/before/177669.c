static void write_ivf_frame_size(FILE *const outfile, const size_t size) {
 char header[4];
  mem_put_le32(header, static_cast<unsigned int>(size));
 (void)fwrite(header, 1, 4, outfile);
}