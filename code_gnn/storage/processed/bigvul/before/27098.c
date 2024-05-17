static int get_appcontext_input_count_offset(void)
{
#define low_offset		offsetof(struct _XtAppStruct, __maxed__nfds)
#define high_offset		offsetof(struct _XtAppStruct, __maybe__input_max)
#define n_offsets_max	(high_offset - low_offset)/2
  int i, ofs, n_offsets = 0;
  int offsets[n_offsets_max] = { 0, };

#define n_inputs_max	4  
  int fd, id, n_inputs = 0;
  struct { int fd, id; } inputs[n_inputs_max] = { 0, };

  if ((fd = open("/dev/null", O_WRONLY)) < 0)
	return 0;
  if ((id = add_appcontext_input(fd, 0)) < 0) {
	close(fd);
	return 0;
  }
  inputs[n_inputs].fd = fd;
  inputs[n_inputs].id = id;
  n_inputs++;

  for (ofs = low_offset; ofs < high_offset; ofs += 2) {
	if (get_appcontext_input_count_at(ofs) == 1)
	  offsets[n_offsets++] = ofs;
  }

  while (n_inputs < n_inputs_max) {
	if ((fd = open("/dev/null", O_WRONLY)) < 0)
	  break;
	if ((id = add_appcontext_input(fd, n_inputs)) < 0) {
	  close(fd);
	  break;
	}
	inputs[n_inputs].fd = fd;
	inputs[n_inputs].id = id;
	n_inputs++;

	int n = 0;
	for (i = 0; i < n_offsets; i++) {
	  if (get_appcontext_input_count_at(offsets[i]) == n_inputs)
		offsets[n++] = offsets[i];
	}
	for (i = n; i < n_offsets; i++)
	  offsets[i] = 0;
	n_offsets = n;
  }

  for (i = 0; i < n_inputs; i++) {
	XtRemoveInput(inputs[i].id);
	close(inputs[i].fd);
  }

  if (n_offsets == 1)
	return offsets[0];

#undef n_fds_max
#undef n_offsets_max
#undef high_offset
#undef low_offset
  return 0;
}