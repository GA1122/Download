COMPS_Object* __comps_objrtree_get(COMPS_ObjRTree * rt, const char * key) {
    COMPS_HSList * subnodes;
    COMPS_HSListItem * it = NULL;
    COMPS_ObjRTreeData * rtdata;
    unsigned int offset, len, x;
    char found, ended;

    len = strlen(key);
    offset = 0;
    subnodes = rt->subnodes;

    while (offset != len) {
        found = 0;
        for (it = subnodes->first; it != NULL; it=it->next) {
            if (((COMPS_ObjRTreeData*)it->data)->key[0] == key[offset]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return NULL;
        }
        rtdata = (COMPS_ObjRTreeData*)it->data;

        for (x=1; ;x++) {
            ended=0;
            if (x == strlen(rtdata->key)) ended += 1;
            if (x == len-offset) ended += 2;
            if (ended != 0) break;
            if (key[offset+x] != rtdata->key[x]) break;
        }
        if (ended == 3) {
            return rtdata->data;
        }
        else if (ended == 1) offset+=x;
        else {
            return NULL;
        }
        subnodes = ((COMPS_ObjRTreeData*)it->data)->subnodes;
    }
    if (it != NULL) {
        return ((COMPS_ObjRTreeData*)it->data)->data;
    }
    else {
        return NULL;
    }
}