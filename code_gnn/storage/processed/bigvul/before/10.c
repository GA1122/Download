static int samldb_fill_object(struct samldb_ctx *ac)
{
	struct ldb_context *ldb = ldb_module_get_ctx(ac->module);
	int ret;

	 
	switch(ac->type) {
	case SAMLDB_TYPE_USER: {
		struct ldb_control *rodc_control = ldb_request_get_control(ac->req,
									   LDB_CONTROL_RODC_DCPROMO_OID);
		if (rodc_control != NULL) {
			 
			rodc_control->critical = false;
			ret = samldb_add_step(ac, samldb_rodc_add);
			if (ret != LDB_SUCCESS) return ret;
		}

		 
		ret = samldb_add_step(ac, samldb_check_sAMAccountName);
		if (ret != LDB_SUCCESS) return ret;

		ret = samldb_add_step(ac, samldb_add_entry);
		if (ret != LDB_SUCCESS) return ret;
		break;
	}

	case SAMLDB_TYPE_GROUP: {
		 
		ret = samldb_add_step(ac, samldb_check_sAMAccountName);
		if (ret != LDB_SUCCESS) return ret;

		ret = samldb_add_step(ac, samldb_add_entry);
		if (ret != LDB_SUCCESS) return ret;
		break;
	}

	case SAMLDB_TYPE_CLASS: {
		const struct ldb_val *rdn_value, *def_obj_cat_val;
		unsigned int v = ldb_msg_find_attr_as_uint(ac->msg, "objectClassCategory", -2);

		 
		if (!ldb_msg_find_element(ac->msg, "subClassOf")) {
			ret = ldb_msg_add_string(ac->msg, "subClassOf", "top");
			if (ret != LDB_SUCCESS) return ret;
		}

		ret = samdb_find_or_add_attribute(ldb, ac->msg,
						  "rdnAttId", "cn");
		if (ret != LDB_SUCCESS) return ret;

		 
		if (check_rodc_critical_attribute(ac->msg)) {
			ldb_asprintf_errstring(ldb, "Refusing schema add of %s - cannot combine critical class with RODC filtering",
					       ldb_dn_get_linearized(ac->msg->dn));
			return LDB_ERR_UNWILLING_TO_PERFORM;
		}

		rdn_value = ldb_dn_get_rdn_val(ac->msg->dn);
		if (rdn_value == NULL) {
			return ldb_operr(ldb);
		}
		if (!ldb_msg_find_element(ac->msg, "lDAPDisplayName")) {
			 
			ret = ldb_msg_add_string(ac->msg, "lDAPDisplayName",
				samdb_cn_to_lDAPDisplayName(ac->msg,
							    (const char *) rdn_value->data));
			if (ret != LDB_SUCCESS) {
				ldb_oom(ldb);
				return ret;
			}
		}

		if (!ldb_msg_find_element(ac->msg, "schemaIDGUID")) {
			struct GUID guid;
			 
			guid = GUID_random();
			ret = dsdb_msg_add_guid(ac->msg, &guid, "schemaIDGUID");
			if (ret != LDB_SUCCESS) {
				ldb_oom(ldb);
				return ret;
			}
		}

		def_obj_cat_val = ldb_msg_find_ldb_val(ac->msg,
						       "defaultObjectCategory");
		if (def_obj_cat_val != NULL) {
			 
			ac->dn = ldb_dn_from_ldb_val(ac, ldb, def_obj_cat_val);
			if (ac->dn == NULL) {
				ldb_set_errstring(ldb,
						  "Invalid DN for 'defaultObjectCategory'!");
				return LDB_ERR_CONSTRAINT_VIOLATION;
			}
		} else {
			 
			ac->dn = ac->msg->dn;

			ret = ldb_msg_add_string(ac->msg, "defaultObjectCategory",
						 ldb_dn_alloc_linearized(ac->msg, ac->dn));
			if (ret != LDB_SUCCESS) {
				ldb_oom(ldb);
				return ret;
			}
		}

		ret = samldb_add_step(ac, samldb_add_entry);
		if (ret != LDB_SUCCESS) return ret;

		 
		ret = samldb_add_step(ac, samldb_find_for_defaultObjectCategory);
		if (ret != LDB_SUCCESS) return ret;

		 
		if (v == -2) {
			 
			ret = samdb_msg_add_uint(ldb, ac->msg, ac->msg, "objectClassCategory", 0);
			if (ret != LDB_SUCCESS) {
				return ret;
			}
		}
		break;
	}

	case SAMLDB_TYPE_ATTRIBUTE: {
		const struct ldb_val *rdn_value;
		struct ldb_message_element *el;
		rdn_value = ldb_dn_get_rdn_val(ac->msg->dn);
		if (rdn_value == NULL) {
			return ldb_operr(ldb);
		}
		if (!ldb_msg_find_element(ac->msg, "lDAPDisplayName")) {
			 
			ret = ldb_msg_add_string(ac->msg, "lDAPDisplayName",
				samdb_cn_to_lDAPDisplayName(ac->msg,
							    (const char *) rdn_value->data));
			if (ret != LDB_SUCCESS) {
				ldb_oom(ldb);
				return ret;
			}
		}

		 
		if (check_rodc_critical_attribute(ac->msg)) {
			ldb_asprintf_errstring(ldb,
					       "samldb: refusing schema add of %s - cannot combine critical attribute with RODC filtering",
					       ldb_dn_get_linearized(ac->msg->dn));
			return LDB_ERR_UNWILLING_TO_PERFORM;
		}

		ret = samdb_find_or_add_attribute(ldb, ac->msg,
						  "isSingleValued", "FALSE");
		if (ret != LDB_SUCCESS) return ret;

		if (!ldb_msg_find_element(ac->msg, "schemaIDGUID")) {
			struct GUID guid;
			 
			guid = GUID_random();
			ret = dsdb_msg_add_guid(ac->msg, &guid, "schemaIDGUID");
			if (ret != LDB_SUCCESS) {
				ldb_oom(ldb);
				return ret;
			}
		}

		el = ldb_msg_find_element(ac->msg, "attributeSyntax");
		if (el) {
			 
			const struct dsdb_syntax *syntax = find_syntax_map_by_ad_oid((const char *)el->values[0].data);
			if (!syntax) {
				DEBUG(9, ("Can't find dsdb_syntax object for attributeSyntax %s\n",
						(const char *)el->values[0].data));
			} else {
				unsigned int v = ldb_msg_find_attr_as_uint(ac->msg, "oMSyntax", 0);
				const struct ldb_val *val = ldb_msg_find_ldb_val(ac->msg, "oMObjectClass");

				if (v == 0) {
					ret = samdb_msg_add_uint(ldb, ac->msg, ac->msg, "oMSyntax", syntax->oMSyntax);
					if (ret != LDB_SUCCESS) {
						return ret;
					}
				}
				if (!val) {
					struct ldb_val val2 = ldb_val_dup(ldb, &syntax->oMObjectClass);
					if (val2.length > 0) {
						ret = ldb_msg_add_value(ac->msg, "oMObjectClass", &val2, NULL);
						if (ret != LDB_SUCCESS) {
							return ret;
						}
					}
				}
			}
		}

		 
		ret = samldb_add_handle_msDS_IntId(ac);
		if (ret != LDB_SUCCESS) return ret;

		ret = samldb_add_step(ac, samldb_add_entry);
		if (ret != LDB_SUCCESS) return ret;
		break;
	}

	default:
		ldb_asprintf_errstring(ldb, "Invalid entry type!");
		return LDB_ERR_OPERATIONS_ERROR;
		break;
	}

	return samldb_first_step(ac);
}