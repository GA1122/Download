 static bool ldap_push_filter(struct asn1_data *data, struct ldb_parse_tree *tree)
 {
 	int i;
 
         switch (tree->operation) {
         case LDB_OP_AND:
         case LDB_OP_OR:
               asn1_push_tag(data, ASN1_CONTEXT(tree->operation==LDB_OP_AND?0:1));
//                if (!asn1_push_tag(data, ASN1_CONTEXT(tree->operation==LDB_OP_AND?0:1))) return false;
                 for (i=0; i<tree->u.list.num_elements; i++) {
                         if (!ldap_push_filter(data, tree->u.list.elements[i])) {
                                 return false;
                         }
                 }
               asn1_pop_tag(data);
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         case LDB_OP_NOT:
               asn1_push_tag(data, ASN1_CONTEXT(2));
//                if (!asn1_push_tag(data, ASN1_CONTEXT(2))) return false;
                 if (!ldap_push_filter(data, tree->u.isnot.child)) {
                         return false;
                 }
               asn1_pop_tag(data);
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         case LDB_OP_EQUALITY:
                  
               asn1_push_tag(data, ASN1_CONTEXT(3));
               asn1_write_OctetString(data, tree->u.equality.attr,
                                     strlen(tree->u.equality.attr));
               asn1_write_OctetString(data, tree->u.equality.value.data,
                                     tree->u.equality.value.length);
               asn1_pop_tag(data);
//                if (!asn1_push_tag(data, ASN1_CONTEXT(3))) return false;
//                if (!asn1_write_OctetString(data, tree->u.equality.attr,
//                                      strlen(tree->u.equality.attr))) return false;
//                if (!asn1_write_OctetString(data, tree->u.equality.value.data,
//                                      tree->u.equality.value.length)) return false;
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         case LDB_OP_SUBSTRING:
 		 
               asn1_push_tag(data, ASN1_CONTEXT(4));
               asn1_write_OctetString(data, tree->u.substring.attr, strlen(tree->u.substring.attr));
               asn1_push_tag(data, ASN1_SEQUENCE(0));
//                if (!asn1_push_tag(data, ASN1_CONTEXT(4))) return false;
//                if (!asn1_write_OctetString(data, tree->u.substring.attr, strlen(tree->u.substring.attr))) return false;
//                if (!asn1_push_tag(data, ASN1_SEQUENCE(0))) return false;
  
                 if (tree->u.substring.chunks && tree->u.substring.chunks[0]) {
                         i = 0;
                         if (!tree->u.substring.start_with_wildcard) {
                               asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(0));
                               asn1_write_DATA_BLOB_LDAPString(data, tree->u.substring.chunks[i]);
                               asn1_pop_tag(data);
//                                if (!asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(0))) return false;
//                                if (!asn1_write_DATA_BLOB_LDAPString(data, tree->u.substring.chunks[i])) return false;
//                                if (!asn1_pop_tag(data)) return false;
                                 i++;
                         }
                         while (tree->u.substring.chunks[i]) {
 				int ctx;
 
 				if (( ! tree->u.substring.chunks[i + 1]) &&
 				    (tree->u.substring.end_with_wildcard == 0)) {
 					ctx = 2;
                                 } else {
                                         ctx = 1;
                                 }
                               asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(ctx));
                               asn1_write_DATA_BLOB_LDAPString(data, tree->u.substring.chunks[i]);
                               asn1_pop_tag(data);
//                                if (!asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(ctx))) return false;
//                                if (!asn1_write_DATA_BLOB_LDAPString(data, tree->u.substring.chunks[i])) return false;
//                                if (!asn1_pop_tag(data)) return false;
                                 i++;
                         }
                 }
               asn1_pop_tag(data);
               asn1_pop_tag(data);
//                if (!asn1_pop_tag(data)) return false;
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         case LDB_OP_GREATER:
                  
               asn1_push_tag(data, ASN1_CONTEXT(5));
               asn1_write_OctetString(data, tree->u.comparison.attr,
                                     strlen(tree->u.comparison.attr));
               asn1_write_OctetString(data, tree->u.comparison.value.data,
                                     tree->u.comparison.value.length);
               asn1_pop_tag(data);
//                if (!asn1_push_tag(data, ASN1_CONTEXT(5))) return false;
//                if (!asn1_write_OctetString(data, tree->u.comparison.attr,
//                                      strlen(tree->u.comparison.attr))) return false;
//                if (!asn1_write_OctetString(data, tree->u.comparison.value.data,
//                                      tree->u.comparison.value.length)) return false;
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         case LDB_OP_LESS:
                  
               asn1_push_tag(data, ASN1_CONTEXT(6));
               asn1_write_OctetString(data, tree->u.comparison.attr,
                                     strlen(tree->u.comparison.attr));
               asn1_write_OctetString(data, tree->u.comparison.value.data,
                                     tree->u.comparison.value.length);
               asn1_pop_tag(data);
//                if (!asn1_push_tag(data, ASN1_CONTEXT(6))) return false;
//                if (!asn1_write_OctetString(data, tree->u.comparison.attr,
//                                      strlen(tree->u.comparison.attr))) return false;
//                if (!asn1_write_OctetString(data, tree->u.comparison.value.data,
//                                      tree->u.comparison.value.length)) return false;
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         case LDB_OP_PRESENT:
                  
               asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(7));
               asn1_write_LDAPString(data, tree->u.present.attr);
               asn1_pop_tag(data);
//                if (!asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(7))) return false;
//                if (!asn1_write_LDAPString(data, tree->u.present.attr)) return false;
//                if (!asn1_pop_tag(data)) return false;
                 return !data->has_error;
  
         case LDB_OP_APPROX:
                  
               asn1_push_tag(data, ASN1_CONTEXT(8));
               asn1_write_OctetString(data, tree->u.comparison.attr,
                                     strlen(tree->u.comparison.attr));
               asn1_write_OctetString(data, tree->u.comparison.value.data,
                                     tree->u.comparison.value.length);
               asn1_pop_tag(data);
//                if (!asn1_push_tag(data, ASN1_CONTEXT(8))) return false;
//                if (!asn1_write_OctetString(data, tree->u.comparison.attr,
//                                      strlen(tree->u.comparison.attr))) return false;
//                if (!asn1_write_OctetString(data, tree->u.comparison.value.data,
//                                      tree->u.comparison.value.length)) return false;
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         case LDB_OP_EXTENDED:
 		 
               asn1_push_tag(data, ASN1_CONTEXT(9));
//                if (!asn1_push_tag(data, ASN1_CONTEXT(9))) return false;
                 if (tree->u.extended.rule_id) {
                       asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(1));
                       asn1_write_LDAPString(data, tree->u.extended.rule_id);
                       asn1_pop_tag(data);
//                        if (!asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(1))) return false;
//                        if (!asn1_write_LDAPString(data, tree->u.extended.rule_id)) return false;
//                        if (!asn1_pop_tag(data)) return false;
                 }
                 if (tree->u.extended.attr) {
                       asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(2));
                       asn1_write_LDAPString(data, tree->u.extended.attr);
                       asn1_pop_tag(data);
//                        if (!asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(2))) return false;
//                        if (!asn1_write_LDAPString(data, tree->u.extended.attr)) return false;
//                        if (!asn1_pop_tag(data)) return false;
                 }
               asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(3));
               asn1_write_DATA_BLOB_LDAPString(data, &tree->u.extended.value);
               asn1_pop_tag(data);
               asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(4));
               asn1_write_uint8(data, tree->u.extended.dnAttributes);
               asn1_pop_tag(data);
               asn1_pop_tag(data);
//                if (!asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(3))) return false;
//                if (!asn1_write_DATA_BLOB_LDAPString(data, &tree->u.extended.value)) return false;
//                if (!asn1_pop_tag(data)) return false;
//                if (!asn1_push_tag(data, ASN1_CONTEXT_SIMPLE(4))) return false;
//                if (!asn1_write_uint8(data, tree->u.extended.dnAttributes)) return false;
//                if (!asn1_pop_tag(data)) return false;
//                if (!asn1_pop_tag(data)) return false;
                 break;
  
         default:
 		return false;
 	}
         return !data->has_error;
  }