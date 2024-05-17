void __comps_mrtree_set(COMPS_MRTree * rt, char * key, size_t len, void * data)
{
    static COMPS_HSListItem *it;
    COMPS_HSList *subnodes;
    COMPS_MRTreeData *rtd;
    static COMPS_MRTreeData *rtdata;

    size_t _len, offset=0;
    unsigned x, found = 0;
    void *ndata;
    char ended; 

    if (rt->subnodes == NULL)
        return;
    if (rt->data_constructor)
        ndata = rt->data_constructor(data);
    else
        ndata = data;

    subnodes = rt->subnodes;
    while (offset != len)
    {
        found = 0;
        for (it = subnodes->first; it != NULL; it=it->next) {
            if (((COMPS_MRTreeData*)it->data)->key[0] == key[offset]) {
                found = 1;
                break;
            }
        }
        if (!found) {  
            rtd = comps_mrtree_data_create(rt, key+offset, ndata);
            comps_hslist_append(subnodes, rtd, 0);
            return;
        } else {
            rtdata = (COMPS_MRTreeData*)it->data;
            ended = 0;
            for (x=1; ;x++) {
                if (rtdata->key[x] == 0) ended += 1;
                if (x == len - offset) ended += 2;
                if (ended != 0) break;
                if (key[offset+x] != rtdata->key[x]) break;
            }
            if (ended == 3) {  
                comps_hslist_append(rtdata->data, ndata, 0);
                return;
            } else if (ended == 2) {  
                comps_hslist_remove(subnodes, it);
                it->next = NULL;
                rtd = comps_mrtree_data_create(rt, key+offset,  ndata);
                comps_hslist_append(subnodes, rtd, 0);
                ((COMPS_MRTreeData*)subnodes->last->data)->subnodes->last = it;
                ((COMPS_MRTreeData*)subnodes->last->data)->subnodes->first = it;
                _len = strlen(key + offset);
                memmove(rtdata->key,rtdata->key+_len, strlen(rtdata->key) - _len);
                rtdata->key[strlen(rtdata->key) - _len] = 0;
                rtdata->key = realloc(rtdata->key,
                                      sizeof(char)* (strlen(rtdata->key)+1));
                return;
            } else if (ended == 1) {  
                subnodes = rtdata->subnodes;
                offset += x;
            } else {  
                void *tmpdata = rtdata->data;
                COMPS_HSList *tmpnodes = rtdata->subnodes;

                int cmpret = strcmp(key+offset+x, rtdata->key+x);
                rtdata->subnodes = comps_hslist_create();
                comps_hslist_init(rtdata->subnodes, NULL, NULL,
                                  &comps_mrtree_data_destroy_v);
                rtdata->data = NULL;
                if (cmpret > 0) {
                    rtd = comps_mrtree_data_create(rt, rtdata->key+x, tmpdata);
                    comps_hslist_destroy(&rtd->subnodes);
                    rtd->subnodes = tmpnodes;
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                    rtd = comps_mrtree_data_create(rt, key+offset+x, ndata);
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                } else {
                    rtd = comps_mrtree_data_create(rt, key+offset+x, ndata);
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                    rtd = comps_mrtree_data_create(rt, rtdata->key+x, tmpdata);
                    comps_hslist_destroy(&rtd->subnodes);
                    rtd->subnodes = tmpnodes;
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                }
                rtdata->key = realloc(rtdata->key, sizeof(char)*(x+1));
                rtdata->key[x] = 0;
                return;
            }
        }
    }
}