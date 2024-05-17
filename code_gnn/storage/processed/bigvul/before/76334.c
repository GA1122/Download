static int chmd_init_decomp(struct mschm_decompressor_p *self,
			    struct mschmd_file *file)
{
  int window_size, window_bits, reset_interval, entry, err;
  struct mspack_system *sys = self->system;
  struct mschmd_sec_mscompressed *sec;
  unsigned char *data;
  off_t length, offset;

  sec = (struct mschmd_sec_mscompressed *) file->section;

   
  err = find_sys_file(self, sec, &sec->content, content_name);
  if (err) return self->error = err;

   
  err = find_sys_file(self, sec, &sec->control, control_name);
  if (err) return self->error = err;

   
  if (sec->control->length < lzxcd_SIZEOF) {
    D(("ControlData file is too short"))
    return self->error = MSPACK_ERR_DATAFORMAT;
  }
  if (!(data = read_sys_file(self, sec->control))) {
    D(("can't read mscompressed control data file"))
    return self->error;
  }

   
  if (EndGetI32(&data[lzxcd_Signature]) != 0x43585A4C) {
    sys->free(data);
    return self->error = MSPACK_ERR_SIGNATURE;
  }

   
  switch (EndGetI32(&data[lzxcd_Version])) {
  case 1:
    reset_interval = EndGetI32(&data[lzxcd_ResetInterval]);
    window_size    = EndGetI32(&data[lzxcd_WindowSize]);
    break;
  case 2:
    reset_interval = EndGetI32(&data[lzxcd_ResetInterval]) * LZX_FRAME_SIZE;
    window_size    = EndGetI32(&data[lzxcd_WindowSize])    * LZX_FRAME_SIZE;
    break;
  default:
    D(("bad controldata version"))
    sys->free(data);
    return self->error = MSPACK_ERR_DATAFORMAT;
  }

   
  sys->free(data);

   
  switch (window_size) {
  case 0x008000: window_bits = 15; break;
  case 0x010000: window_bits = 16; break;
  case 0x020000: window_bits = 17; break;
  case 0x040000: window_bits = 18; break;
  case 0x080000: window_bits = 19; break;
  case 0x100000: window_bits = 20; break;
  case 0x200000: window_bits = 21; break;
  default:
    D(("bad controldata window size"))
    return self->error = MSPACK_ERR_DATAFORMAT;
  }

   
  if (reset_interval == 0 || reset_interval % LZX_FRAME_SIZE) {
    D(("bad controldata reset interval"))
    return self->error = MSPACK_ERR_DATAFORMAT;
  }

   
  entry = file->offset / reset_interval;
   
  entry *= reset_interval / LZX_FRAME_SIZE;

   
  if (read_reset_table(self, sec, entry, &length, &offset)) {
     
    length += reset_interval - 1;
    length &= -reset_interval;
  }
  else {
     
    entry = 0;
    offset = 0;
    err = read_spaninfo(self, sec, &length);
  }
  if (err) return self->error = err;

   
  self->d->inoffset = file->section->chm->sec0.offset + sec->content->offset + offset;

   
  self->d->offset = entry * LZX_FRAME_SIZE;
  length -= self->d->offset;

   
  self->d->state = lzxd_init(&self->d->sys, self->d->infh,
			     (struct mspack_file *) self, window_bits,
			     reset_interval / LZX_FRAME_SIZE,
			     4096, length, 0);
  if (!self->d->state) self->error = MSPACK_ERR_NOMEMORY;
  return self->error;
}