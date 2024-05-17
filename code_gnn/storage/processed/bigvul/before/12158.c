XMLRPC_VALUE_TYPE_EASY XMLRPC_GetValueTypeEasy (XMLRPC_VALUE value) {
	if (value) {
		switch (value->type) {
		case xmlrpc_vector:
			switch (value->v->type) {
			case xmlrpc_vector_none:
				return xmlrpc_type_none;
			case xmlrpc_vector_struct:
				return xmlrpc_type_struct;
			case xmlrpc_vector_mixed:
				return xmlrpc_type_mixed;
			case xmlrpc_vector_array:
				return xmlrpc_type_array;
			}
		default:
			 
			return(XMLRPC_VALUE_TYPE_EASY) value->type;
		}
	}
	return xmlrpc_none;
}
