static u32 store_or_aggregate(GF_StreamContext *sc, GF_Command *com, GF_List *commands, Bool *has_modif)
{
#ifndef GPAC_DISABLE_VRML
	u32 i, count, j, nb_fields;
	GF_CommandField *field, *check_field;

	 
	if (node_in_commands_subtree(com->node, commands)) return 0;

	 
	count = gf_list_count(commands);
	for (i=0; i<count; i++) {
		GF_Command *check = gf_list_get(commands, i);

		if (sc->streamType == GF_STREAM_SCENE) {
			Bool check_index=0;
			Bool original_is_index = 0;
			Bool apply;
			switch (com->tag) {
			case GF_SG_INDEXED_REPLACE:
				check_index=1;
			case GF_SG_MULTIPLE_INDEXED_REPLACE:
			case GF_SG_FIELD_REPLACE:
			case GF_SG_MULTIPLE_REPLACE:
				if (check->node != com->node) break;
				 
				if (check_index) {
					if (check->tag == GF_SG_INDEXED_REPLACE) {}
					else if (check->tag == GF_SG_INDEXED_INSERT) {
						original_is_index = 1;
					}
					else {
						break;
					}
				} else {
					if (check->tag != com->tag) break;
				}
				nb_fields = gf_list_count(com->command_fields);
				if (gf_list_count(check->command_fields) != nb_fields) break;
				apply=1;
				for (j=0; j<nb_fields; j++) {
					field = gf_list_get(com->command_fields, j);
					check_field = gf_list_get(check->command_fields, j);
					if ((field->pos != check_field->pos) || (field->fieldIndex != check_field->fieldIndex)) {
						apply=0;
						break;
					}
				}
				 
				if (apply) {
					 
					if (original_is_index) com->tag = GF_SG_INDEXED_INSERT;

					gf_sg_command_del((GF_Command *)check);
					gf_list_rem(commands, i);
					if (has_modif) *has_modif = 1;
					return 1;
				}
				break;

			case GF_SG_NODE_REPLACE:
				if (check->tag != GF_SG_NODE_REPLACE) {
					break;
				}
				 
				if (gf_node_get_id(check->node) != gf_node_get_id(com->node) ) {
					break;
				}
				 
				gf_sg_command_del((GF_Command *)check);
				gf_list_rem(commands, i);
				if (has_modif) *has_modif = 1;
				return 1;

			case GF_SG_INDEXED_DELETE:
				 
				if (check->tag != GF_SG_INDEXED_INSERT) break;
				if (com->node != check->node) break;
				field = gf_list_get(com->command_fields, 0);
				check_field = gf_list_get(check->command_fields, 0);
				if (!field || !check_field) break;
				if (field->pos != check_field->pos) break;
				if (field->fieldIndex != check_field->fieldIndex) break;

				gf_sg_command_del((GF_Command *)check);
				gf_list_rem(commands, i);
				if (has_modif) *has_modif = 1;
				return 2;

			default:
				GF_LOG(GF_LOG_ERROR, GF_LOG_SCENE, ("[Scene Manager] Stream Aggregation not implemented for command - aggregating on main scene\n"));
				break;
			}
		}
	}
	 
	if (has_modif) *has_modif=1;
#endif
	return 1;
}