PHP_METHOD(SoapClient, __getTypes)
{
	sdlPtr sdl;
	HashPosition pos;

	FETCH_THIS_SDL(sdl);
	
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (sdl) {
		sdlTypePtr *type;
		smart_str buf = {0};

		array_init(return_value);
		if (sdl->types) {
			zend_hash_internal_pointer_reset_ex(sdl->types, &pos);
			while (zend_hash_get_current_data_ex(sdl->types, (void **)&type, &pos) != FAILURE) {
				type_to_string((*type), &buf, 0);
				add_next_index_stringl(return_value, buf.c, buf.len, 1);
				smart_str_free(&buf);
				zend_hash_move_forward_ex(sdl->types, &pos);
			}
		}
	}
}
