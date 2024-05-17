static int print_file(sc_card_t *in_card, const sc_file_t *file,
	const sc_path_t *path, int depth)
{
	int r;
	const char *tmps;

	for (r = 0; r < depth; r++)
		printf("  ");
	printf("%s ", sc_print_path(path));
	if (file->namelen) {
		printf("[");
		util_print_binary(stdout, file->name, file->namelen);
		printf("] ");
	}
	switch (file->type) {
	case SC_FILE_TYPE_WORKING_EF:
		tmps = "wEF";
		break;
	case SC_FILE_TYPE_INTERNAL_EF:
		tmps = "iEF";
		break;
	case SC_FILE_TYPE_DF:
		tmps = "DF";
		break;
	default:
		tmps = "unknown";
		break;
	}
	printf("type: %s, ", tmps);
	if (file->type != SC_FILE_TYPE_DF) {
		const id2str_t ef_type_name[] = {
			{ SC_FILE_EF_TRANSPARENT,         "transparent"           },
			{ SC_FILE_EF_LINEAR_FIXED,        "linear-fixed"          },
			{ SC_FILE_EF_LINEAR_FIXED_TLV,    "linear-fixed (TLV)"    },
			{ SC_FILE_EF_LINEAR_VARIABLE,     "linear-variable"       },
			{ SC_FILE_EF_LINEAR_VARIABLE_TLV, "linear-variable (TLV)" },
			{ SC_FILE_EF_CYCLIC,              "cyclic"                },
			{ SC_FILE_EF_CYCLIC_TLV,          "cyclic (TLV)"          },
			{ 0, NULL }
		};
		const char *ef_type = "unknown";

		for (r = 0; ef_type_name[r].str != NULL; r++)
			if (file->ef_structure == ef_type_name[r].id)
				ef_type = ef_type_name[r].str;

		printf("ef structure: %s, ", ef_type);
	}
	printf("size: %lu\n", (unsigned long) file->size);
	for (r = 0; r < depth; r++)
		printf("  ");
	if (file->type == SC_FILE_TYPE_DF) {
		const id2str_t ac_ops_df[] = {
			{ SC_AC_OP_SELECT,       "select" },
			{ SC_AC_OP_LOCK,         "lock"   },
			{ SC_AC_OP_DELETE,       "delete" },
			{ SC_AC_OP_CREATE,       "create" },
			{ SC_AC_OP_REHABILITATE, "rehab"  },
			{ SC_AC_OP_INVALIDATE,   "inval"  },
			{ SC_AC_OP_LIST_FILES,   "list"   },
			{ 0, NULL }
		};

		for (r = 0; ac_ops_df[r].str != NULL; r++)
			printf("%s[%s] ", ac_ops_df[r].str,
					 util_acl_to_str(sc_file_get_acl_entry(file, ac_ops_df[r].id)));
	}
	else {
		const id2str_t ac_ops_ef[] = {
			{ SC_AC_OP_READ,         "read"   },
			{ SC_AC_OP_UPDATE,       "update" },
			{ SC_AC_OP_ERASE,        "erase"  },
			{ SC_AC_OP_WRITE,        "write"  },
			{ SC_AC_OP_REHABILITATE, "rehab"  },
			{ SC_AC_OP_INVALIDATE,   "inval"  },
			{ 0, NULL }
		};

		for (r = 0; ac_ops_ef[r].str != NULL; r++)
			printf("%s[%s] ", ac_ops_ef[r].str,
					util_acl_to_str(sc_file_get_acl_entry(file, ac_ops_ef[r].id)));
	}

	if (file->sec_attr_len) {
		printf("sec: ");
		 
		util_hex_dump(stdout, file->sec_attr, file->sec_attr_len, ":");
	}
	if (file->prop_attr_len) {
		printf("\n");
		for (r = 0; r < depth; r++)
			printf("  ");
		printf("prop: ");
		util_hex_dump(stdout, file->prop_attr, file->prop_attr_len, ":");
	}
	printf("\n\n");

	if (file->type == SC_FILE_TYPE_DF)
		return 0;

	if (file->ef_structure == SC_FILE_EF_TRANSPARENT) {
		unsigned char *buf;

		if (!(buf = malloc(file->size))) {
			fprintf(stderr, "out of memory");
			return 1;
		}

		r = sc_lock(card);
		if (r == SC_SUCCESS)
			r = sc_read_binary(in_card, 0, buf, file->size, 0);
		sc_unlock(card);
		if (r > 0)
			util_hex_dump_asc(stdout, buf, r, 0);
		free(buf);
	} else {
		unsigned char buf[256];
		int i;

		for (i=0; i < file->record_count; i++) {
			printf("Record %d\n", i);
			r = sc_lock(card);
			if (r == SC_SUCCESS)
				r = sc_read_record(in_card, i, buf, 256, 0);
			sc_unlock(card);
			if (r > 0)
				util_hex_dump_asc(stdout, buf, r, 0);
		}
	}
	return 0;
}