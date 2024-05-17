static int samldb_add_handle_msDS_IntId(struct samldb_ctx *ac)
{
	int ret;
	bool id_exists;
	uint32_t msds_intid;
	int32_t system_flags;
	struct ldb_context *ldb;
	struct ldb_result *ldb_res;
	struct ldb_dn *schema_dn;
	struct samldb_msds_intid_persistant *msds_intid_struct;
	struct dsdb_schema *schema;

	ldb = ldb_module_get_ctx(ac->module);
	schema_dn = ldb_get_schema_basedn(ldb);

	 
	if (ldb_request_get_control(ac->req,
				    DSDB_CONTROL_REPLICATED_UPDATE_OID)) {
		return LDB_SUCCESS;
	}

	 
	if (ldb_msg_find_element(ac->msg, "msDS-IntId")) {
		return LDB_ERR_UNWILLING_TO_PERFORM;
	}

	 
	if (ldb_request_get_control(ac->req, LDB_CONTROL_RELAX_OID)) {
		return LDB_SUCCESS;
	}

	 
	if (dsdb_functional_level(ldb) < DS_DOMAIN_FUNCTION_2003) {
		return LDB_SUCCESS;
	}

	 
	system_flags = ldb_msg_find_attr_as_int(ac->msg, "systemFlags", 0);
	if (system_flags & SYSTEM_FLAG_SCHEMA_BASE_OBJECT) {
		return LDB_SUCCESS;
	}
	schema = dsdb_get_schema(ldb, NULL);
	if (!schema) {
		ldb_debug_set(ldb, LDB_DEBUG_FATAL,
			      "samldb_schema_info_update: no dsdb_schema loaded");
		DEBUG(0,(__location__ ": %s\n", ldb_errstring(ldb)));
		return ldb_operr(ldb);
	}

	msds_intid_struct = (struct samldb_msds_intid_persistant*) ldb_get_opaque(ldb, SAMLDB_MSDS_INTID_OPAQUE);
	if (!msds_intid_struct) {
		msds_intid_struct = talloc(ldb, struct samldb_msds_intid_persistant);
		 
		msds_intid = generate_random() % 0X3FFFFFFF;
		msds_intid += 0x80000000;
		msds_intid_struct->msds_intid = msds_intid;
		msds_intid_struct->usn = schema->loaded_usn;
		DEBUG(2, ("No samldb_msds_intid_persistant struct, allocating a new one\n"));
	} else {
		msds_intid = msds_intid_struct->msds_intid;
	}

	 
	do {
		uint64_t current_usn;
		msds_intid++;
		if (msds_intid > 0xBFFFFFFF) {
			msds_intid = 0x80000001;
		}
		 
		if (dsdb_attribute_by_attributeID_id(schema, msds_intid)) {
			msds_intid = generate_random() % 0X3FFFFFFF;
			msds_intid += 0x80000000;
			continue;
		}

		ret = dsdb_module_load_partition_usn(ac->module, schema_dn,
						     &current_usn, NULL, NULL);
		if (ret != LDB_SUCCESS) {
			ldb_debug_set(ldb, LDB_DEBUG_ERROR,
				      __location__": Searching for schema USN failed: %s\n",
				      ldb_errstring(ldb));
			return ldb_operr(ldb);
		}

		 
		if (current_usn > msds_intid_struct->usn) {
			 

			DEBUG(2, ("Schema has changed, searching the database for the unicity of %d\n",
					msds_intid));

			ret = dsdb_module_search(ac->module, ac,
						&ldb_res,
						schema_dn, LDB_SCOPE_ONELEVEL, NULL,
						DSDB_FLAG_NEXT_MODULE,
						ac->req,
						"(msDS-IntId=%d)", msds_intid);
			if (ret != LDB_SUCCESS) {
				ldb_debug_set(ldb, LDB_DEBUG_ERROR,
					__location__": Searching for msDS-IntId=%d failed - %s\n",
					msds_intid,
					ldb_errstring(ldb));
				return ldb_operr(ldb);
			}
			id_exists = (ldb_res->count > 0);
			talloc_free(ldb_res);
		} else {
			id_exists = 0;
		}

	} while(id_exists);
	msds_intid_struct->msds_intid = msds_intid;
	ldb_set_opaque(ldb, SAMLDB_MSDS_INTID_OPAQUE, msds_intid_struct);

	return samdb_msg_add_int(ldb, ac->msg, ac->msg, "msDS-IntId",
				 msds_intid);
}
