int main(void)
{
  char *str;
  int i, id;

  id_init();

#define N_CELLS_PER_SLOT 8
#define N_STRINGS ((2 * N_CELLS_PER_SLOT) + 3)
  char *strings[N_STRINGS];
  int ids[N_STRINGS];

  for (i = 0; i < N_STRINGS; i++) {
	str = malloc(10);
	sprintf(str, "%d", i);
	strings[i] = str;
	id = id_create(str);
	if (id < 0) {
	  fprintf(stderr, "ERROR: failed to allocate ID for '%s'\n", str);
	  return 1;
	}
	ids[i] = id;
  }

  id = ids[N_CELLS_PER_SLOT / 2];
  str = id_lookup(id);
  printf("str(%d) : '%s'\n", id, str);

  id = ids[N_CELLS_PER_SLOT];
  if (id_remove(id) < 0) {
	fprintf(stderr, "ERROR: failed to unlink ID %d\n", id);
	return 1;
  }
  ids[N_CELLS_PER_SLOT] = 0;

  for (i = 0; i < N_CELLS_PER_SLOT; i++) {
	id = ids[N_CELLS_PER_SLOT + i];
	if (id && id_remove(id) < 0) {
	  fprintf(stderr, "ERROR: failed to unlink ID %d from slot 1\n", id);
	  return 1;
	}
	ids[N_CELLS_PER_SLOT + i] = 0;
  }

  id = ids[2 * N_CELLS_PER_SLOT];
  str = id_lookup(id);
  printf("str(%d) : '%s'\n", id, str);

  for (i = 0; i < N_STRINGS; i++) {
	id = ids[i];
	if (id && id_remove(id) < 0) {
	  fprintf(stderr, "ERROR: failed to unlink ID %d for final cleanup\n", id);
	  return 1;
	}
  }

  for (i = 0; i < N_STRINGS; i++)
	free(strings[i]);

  id_kill();
  return 0;
}
