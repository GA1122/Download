 static void mark_object(struct object *obj, struct strbuf *path,
			const char *name, void *data)
// static void mark_object(struct object *obj, const char *name, void *data)
  {
  	update_progress(data);
  }