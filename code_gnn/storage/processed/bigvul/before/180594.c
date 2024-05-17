 static void show_object(struct object *object, struct strbuf *path,
			const char *last, void *data)
// static void show_object(struct object *object, const char *name, void *data)
  {
  	struct bitmap *base = data;
  	int bitmap_pos;
  
  	bitmap_pos = bitmap_position(object->oid.hash);
  
	if (bitmap_pos < 0) {
		char *name = path_name(path, last);
// 	if (bitmap_pos < 0)
  		bitmap_pos = ext_index_add_object(object, name);
		free(name);
	}
  
  	bitmap_set(base, bitmap_pos);
  }