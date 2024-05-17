void __comps_objmrtree_set(COMPS_ObjMRTree *rt, char *key,
                           size_t len, COMPS_Object *ndata) {
    static COMPS_HSListItem *it;
    COMPS_HSList *subnodes;
    COMPS_ObjMRTreeData *rtd;
    static COMPS_ObjMRTreeData *rtdata;

    size_t _len, offset=0;
    unsigned x, found = 0;
    char ended; 

    if (rt->subnodes == NULL)
        return;

    subnodes = rt->subnodes;
    while (offset != len)
    {
        found = 0;
        for (it = subnodes->first; it != NULL; it = it->next) {
            if (((COMPS_ObjMRTreeData*)it->data)->key[0] == key[offset]) {
                found = 1;
                break;
            }
        }
        if (!found) {  
            rtd = comps_objmrtree_data_create(key+offset, ndata);
            comps_hslist_append(subnodes, rtd, 0);
            rt->len++;
            return;
        } else {
            rtdata = (COMPS_ObjMRTreeData*)it->data;
            ended = 0;
            for (x=1; ;x++) {
                if (rtdata->key[x] == 0) ended += 1;
                if (x == len - offset) ended += 2;
                if (ended != 0) break;
                if (key[offset+x] != rtdata->key[x]) break;
            }
            if (ended == 3) {  
                comps_objlist_append_x(rtdata->data, ndata);
                rt->len++;
                return;
            } else if (ended == 2) {  
                comps_hslist_remove(subnodes, it);
                it->next = NULL;
                rtd = comps_objmrtree_data_create(key+offset, ndata);
                comps_hslist_append(subnodes, rtd, 0);
                ((COMPS_ObjMRTreeData*)subnodes->last->data)->subnodes->last = it;
                ((COMPS_ObjMRTreeData*)subnodes->last->data)->subnodes->first = it;
                _len = strlen(key + offset);
                memmove(rtdata->key,rtdata->key + _len,
                                    strlen(rtdata->key) - _len);
                rtdata->key[strlen(rtdata->key) - _len] = 0;
                rtdata->key = realloc(rtdata->key,
                                      sizeof(char)* (strlen(rtdata->key)+1));
                rt->len++;
                return;
            } else if (ended == 1) {  
                subnodes = rtdata->subnodes;
                offset += x;
            } else {  
                COMPS_ObjList *tmpdata = rtdata->data;
                COMPS_HSList *tmphslist = rtdata->subnodes;

                rtdata->subnodes = comps_hslist_create();
                comps_hslist_init(rtdata->subnodes, NULL, NULL,
                                  &comps_objmrtree_data_destroy_v);
                int cmpret = strcmp(key+offset+x, rtdata->key+x);
                rtdata->data = COMPS_OBJECT_CREATE(COMPS_ObjList, NULL);

                if (cmpret > 0) {
                    rtd = comps_objmrtree_data_create(rtdata->key+x,
                                                      (COMPS_Object*)tmpdata);
                    comps_hslist_destroy(&rtd->subnodes);
                    rtd->subnodes = tmphslist;

                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                    rtd = comps_objmrtree_data_create(key+offset+x,
                                                     (COMPS_Object*)ndata);
                    comps_hslist_append(rtdata->subnodes, rtd, 0);

                } else {
                    rtd = comps_objmrtree_data_create(key+offset+x,
                                                     (COMPS_Object*)ndata);
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                    rtd = comps_objmrtree_data_create(rtdata->key+x,
                                                     (COMPS_Object*)tmpdata);
                    comps_hslist_destroy(&rtd->subnodes);
                    rtd->subnodes = tmphslist;
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                }
                rtdata->key = realloc(rtdata->key, sizeof(char)*(x+1));
                rtdata->key[x] = 0;
                rt->len++;
                return;
            }
        }
    }
}