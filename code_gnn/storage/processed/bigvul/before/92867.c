GF_Err gf_sm_aggregate(GF_SceneManager *ctx, u16 ESID)
{
	GF_Err e;
	u32 i, stream_count;
#ifndef GPAC_DISABLE_VRML
	u32 j;
	GF_AUContext *au;
	GF_Command *com;
#endif

	e = GF_OK;

#if DEBUG_RAP
	com_count = 0;
	stream_count = gf_list_count(ctx->streams);
	for (i=0; i<stream_count; i++) {
		GF_StreamContext *sc = (GF_StreamContext *)gf_list_get(ctx->streams, i);
		if (sc->streamType == GF_STREAM_SCENE) {
			au_count = gf_list_count(sc->AUs);
			for (j=0; j<au_count; j++) {
				au = (GF_AUContext *)gf_list_get(sc->AUs, j);
				com_count += gf_list_count(au->commands);
			}
		}
	}
	GF_LOG(GF_LOG_INFO, GF_LOG_SCENE, ("[SceneManager] Making RAP with %d commands\n", com_count));
#endif

	stream_count = gf_list_count(ctx->streams);
	for (i=0; i<stream_count; i++) {
		GF_AUContext *carousel_au;
		GF_List *carousel_commands;
		GF_StreamContext *aggregate_on_stream;
		GF_StreamContext *sc = (GF_StreamContext *)gf_list_get(ctx->streams, i);
		if (ESID && (sc->ESID!=ESID)) continue;

		 
		carousel_au = NULL;
		carousel_commands = NULL;
		aggregate_on_stream = sc->aggregate_on_esid ? gf_sm_get_stream(ctx, sc->aggregate_on_esid) : NULL;
		if (aggregate_on_stream==sc) {
			carousel_commands = gf_list_new();
		} else if (aggregate_on_stream) {
			if (!gf_list_count(aggregate_on_stream->AUs)) {
				carousel_au = gf_sm_stream_au_new(aggregate_on_stream, 0, 0, 1);
			} else {
				 
				assert(gf_list_count(aggregate_on_stream->AUs)==1);
				carousel_au = gf_list_get(aggregate_on_stream->AUs, 0);
			}
			carousel_commands = carousel_au->commands;
		}
		 
#ifndef GPAC_DISABLE_VRML
		if (sc->streamType == GF_STREAM_SCENE) {
			Bool has_modif = 0;
			 
			Bool base_stream_found = 0;

			 
			if (sc->objectType == GPAC_OTI_SCENE_DIMS) base_stream_found = 1;

			 
			while (gf_list_count(sc->AUs)) {
				u32 count;
				au = (GF_AUContext *) gf_list_get(sc->AUs, 0);
				gf_list_rem(sc->AUs, 0);

				 
				if (au->flags & GF_SM_AU_NOT_AGGREGATED) {
					gf_sm_au_del(sc, au);
					continue;
				}

				count = gf_list_count(au->commands);

				for (j=0; j<count; j++) {
					u32 store=0;
					com = gf_list_get(au->commands, j);
					if (!base_stream_found) {
						switch (com->tag) {
						case GF_SG_SCENE_REPLACE:
						case GF_SG_LSR_NEW_SCENE:
						case GF_SG_LSR_REFRESH_SCENE:
							base_stream_found = 1;
							break;
						}
					}

					 

					 
					if (base_stream_found || !sc->aggregate_on_esid) {
						store = 0;
					}
					 
					else {
						switch (com->tag) {
						 
						case GF_SG_ROUTE_REPLACE:
						case GF_SG_ROUTE_DELETE:
						case GF_SG_ROUTE_INSERT:
						case GF_SG_PROTO_INSERT:
						case GF_SG_PROTO_DELETE:
						case GF_SG_PROTO_DELETE_ALL:
						case GF_SG_GLOBAL_QUANTIZER:
						case GF_SG_LSR_RESTORE:
						case GF_SG_LSR_SAVE:
						case GF_SG_LSR_SEND_EVENT:
						case GF_SG_LSR_CLEAN:
							 
							store = 1;
							break;
						 
						default:
							 
							assert(carousel_commands);
							store = store_or_aggregate(sc, com, carousel_commands, &has_modif);
							break;
						}
					}

					switch (store) {
					 
					case 2:
						gf_list_rem(au->commands, j);
						j--;
						count--;
						gf_sg_command_del((GF_Command *)com);
						break;
					 
					case 1:
						gf_list_insert(carousel_commands, com, 0);
						gf_list_rem(au->commands, j);
						j--;
						count--;
						break;
					 
					default:
						e = gf_sg_command_apply(ctx->scene_graph, com, 0);
						break;
					}
				}
				gf_sm_au_del(sc, au);
			}

			 
			if (base_stream_found) {
				au = gf_sm_stream_au_new(sc, 0, 0, 1);

				switch (sc->objectType) {
				case GPAC_OTI_SCENE_BIFS:
				case GPAC_OTI_SCENE_BIFS_V2:
					com = gf_sg_command_new(ctx->scene_graph, GF_SG_SCENE_REPLACE);
					break;
				case GPAC_OTI_SCENE_LASER:
					com = gf_sg_command_new(ctx->scene_graph, GF_SG_LSR_NEW_SCENE);
					break;
				case GPAC_OTI_SCENE_DIMS:
				 
				default:
					com = NULL;
					break;
				}

				if (com) {
					com->node = ctx->scene_graph->RootNode;
					ctx->scene_graph->RootNode = NULL;
					gf_list_del(com->new_proto_list);
					com->new_proto_list = ctx->scene_graph->protos;
					ctx->scene_graph->protos = NULL;
					 
					com->aggregated = 1;
					gf_list_add(au->commands, com);
				}
			}
			 
			else if (carousel_commands) {
				 
				if (!carousel_au) {
					carousel_au = gf_sm_stream_au_new(sc, 0, 0, 1);
					gf_list_del(carousel_au->commands);
					carousel_au->commands = carousel_commands;
				}
				carousel_au->flags |= GF_SM_AU_RAP | GF_SM_AU_CAROUSEL;
				if (has_modif) carousel_au->flags |= GF_SM_AU_MODIFIED;
			}
		}
#endif
	}
	return e;
}
