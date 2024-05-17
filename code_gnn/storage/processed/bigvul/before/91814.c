void __comps_rtree_set(COMPS_RTree * rt, char * key, size_t len, void * data)
{
    COMPS_HSListItem *it, *lesser;
    COMPS_HSList *subnodes;
    COMPS_RTreeData *rtd;
    static COMPS_RTreeData *rtdata;

    size_t offset=0, _len;
    unsigned x, found = 0;
    void *ndata;
    char ended; 

    if (rt->subnodes == NULL)
        return;
    if (rt->data_constructor) {
        ndata = rt->data_constructor(data);
    } else {
        ndata = data;
    }

    subnodes = rt->subnodes;
    while (offset != len)
    {
        found = 0;
        lesser = NULL;
        for (it = subnodes->first; it != NULL; it=it->next) {
            if (((COMPS_RTreeData*)it->data)->key[0] == key[offset]) {
                found = 1;
                break;
            } else if (((COMPS_RTreeData*)it->data)->key[0] < key[offset]) {
                lesser = it;
            }
        }
        if (!found) {  
            rtd = comps_rtree_data_create(rt, key+offset, ndata);
            if (!lesser) {
                comps_hslist_prepend(subnodes, rtd, 0);
            } else {
                comps_hslist_insert_after(subnodes, lesser, rtd, 0);
            }
            return;
        } else {
            rtdata = (COMPS_RTreeData*)it->data;
            ended = 0;
            for (x=1; ;x++) {
                if (rtdata->key[x] == 0) ended += 1;
                if (x == len - offset) ended += 2;
                if (ended != 0) break;
                if (key[offset+x] != rtdata->key[x]) break;
            }
            if (ended == 3) {  
                rt->data_destructor(rtdata->data);
                rtdata->data = ndata;
                return;
            } else if (ended == 2) {  
                comps_hslist_remove(subnodes, it);
                it->next = NULL;
                rtd = comps_rtree_data_create_n(rt, key+offset,
                                                len-offset, ndata);
                comps_hslist_append(subnodes, rtd, 0);
                ((COMPS_RTreeData*)subnodes->last->data)->subnodes->last = it;
                ((COMPS_RTreeData*)subnodes->last->data)->subnodes->first = it;
                _len = len - offset;

                memmove(rtdata->key,rtdata->key + _len,
                                    strlen(rtdata->key) - _len);
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
                                  &comps_rtree_data_destroy_v);
                rtdata->data = NULL;

                if (cmpret > 0) {
                    rtd = comps_rtree_data_create(rt, rtdata->key+x, tmpdata);
                    comps_hslist_destroy(&rtd->subnodes);
                    rtd->subnodes = tmpnodes;
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                    rtd = comps_rtree_data_create(rt, key+offset+x, ndata);
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                } else {
                    rtd = comps_rtree_data_create(rt, key+offset+x, ndata);
                    comps_hslist_append(rtdata->subnodes, rtd, 0);
                    rtd = comps_rtree_data_create(rt, rtdata->key+x, tmpdata);
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