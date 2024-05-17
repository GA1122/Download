int php_wddx_deserialize_ex(const char *value, size_t vallen, zval *return_value)
{
	wddx_stack stack;
	XML_Parser parser;
	st_entry *ent;
	int retval;

	wddx_stack_init(&stack);
	parser = XML_ParserCreate((XML_Char *) "UTF-8");

	XML_SetUserData(parser, &stack);
	XML_SetElementHandler(parser, php_wddx_push_element, php_wddx_pop_element);
	XML_SetCharacterDataHandler(parser, php_wddx_process_data);

	 
	XML_Parse(parser, (const XML_Char *) value, (int)vallen, 1);

	XML_ParserFree(parser);

	if (stack.top == 1) {
		wddx_stack_top(&stack, (void**)&ent);
		if (Z_ISUNDEF(ent->data)) {
			retval = FAILURE;
		} else {
			ZVAL_COPY(return_value, &ent->data);
			retval = SUCCESS;
		}
	} else {
		retval = FAILURE;
	}

	wddx_stack_destroy(&stack);

	return retval;
}