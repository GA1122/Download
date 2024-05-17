param_with_default_abort(const char *name, int abort) 
{
	char *val = NULL;
	char *next_param_name = NULL;
	MyString subsys = get_mySubSystem()->getName();
	MyString local = get_mySubSystem()->getLocalName();
	MyString subsys_local_name;
	MyString local_name;
	MyString subsys_name;

	StringList sl;
	if (local != "") {
		subsys_local_name = (((subsys + ".") + local) + ".") + name;
		sl.append(subsys_local_name.Value());

		local_name = (local + ".") + name;
		sl.append(local_name.Value());
	}
	subsys_name = (subsys + ".") + name;
	sl.append(subsys_name.Value());
	sl.append(name);

	sl.rewind();
	while(val == NULL && (next_param_name = sl.next())) {
		val = lookup_macro(next_param_name, ConfigTab, TABLESIZE);

		if (val != NULL && val[0] == '\0') {
			return NULL;
		}

		if (val != NULL) {
			break;
		}


		const char * def = param_default_string(next_param_name);
		if (def != NULL) {
			insert(next_param_name, def, ConfigTab, TABLESIZE);
			if (extra_info != NULL) {
				extra_info->AddInternalParam(next_param_name);
			}
			if (def[0] == '\0') {
				return NULL;
			}
            val = const_cast<char*>(def);  
		}
	}

	if (val == NULL) {
		if (abort) {
			EXCEPT("Param name '%s' did not have a definition in any of the "
				   "usual namespaces or default table. Aborting since it MUST "
				   "be defined.\n", name);
		}
		return NULL;
	}


	val = expand_macro( val, ConfigTab, TABLESIZE, NULL, true );

	if( val == NULL ) {
		return NULL;
	}
	
	if ( val[0] == '\0' ) {
		free( val );
		return NULL;
	}

	return val;
}
