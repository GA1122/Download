inline COMPS_HSList* __comps_rtree_all(COMPS_RTree * rt, char keyvalpair) {
    COMPS_HSList *to_process, *ret;
    COMPS_HSListItem *hsit, *oldit;
    size_t x;
    struct Pair {
        char *key;
        void *data;
        COMPS_HSList *subnodes;
    } *pair, *current_pair=NULL; 
    COMPS_RTreePair *rtpair;

    to_process = comps_hslist_create();
    comps_hslist_init(to_process, NULL, NULL, &free);

    ret = comps_hslist_create();
    if (keyvalpair == 0)
        comps_hslist_init(ret, NULL, NULL, &free);
    else if (keyvalpair == 1)
        comps_hslist_init(ret, NULL, NULL, NULL);
    else
        comps_hslist_init(ret, NULL, NULL, &comps_rtree_pair_destroy_v);

    for (hsit = rt->subnodes->first; hsit != NULL; hsit = hsit->next) {
        pair = malloc(sizeof(struct Pair));
        pair->key = __comps_strcpy(((COMPS_RTreeData*)hsit->data)->key);
        pair->data = ((COMPS_RTreeData*)hsit->data)->data;
        pair->subnodes = ((COMPS_RTreeData*)hsit->data)->subnodes;
        comps_hslist_append(to_process, pair, 0);
    }
    while (to_process->first) {
        current_pair = to_process->first->data;
        oldit = to_process->first;
        comps_hslist_remove(to_process, to_process->first);
        if (current_pair->data) {
            if (keyvalpair == 0) {
                comps_hslist_append(ret, __comps_strcpy(current_pair->key), 0);
            } else if (keyvalpair == 1) {
                comps_hslist_append(ret, current_pair->data, 0);
            } else {
                rtpair = malloc(sizeof(COMPS_RTreePair));
                rtpair->key = __comps_strcpy(current_pair->key);
                rtpair->data = current_pair->data;
                comps_hslist_append(ret, rtpair, 0);
            }
        }
        for (hsit = current_pair->subnodes->first, x = 0;
             hsit != NULL; hsit = hsit->next, x++) {
            pair = malloc(sizeof(struct Pair));
            pair->key = __comps_strcat(current_pair->key,
                                       ((COMPS_RTreeData*)hsit->data)->key);
            pair->data = ((COMPS_RTreeData*)hsit->data)->data;
            pair->subnodes = ((COMPS_RTreeData*)hsit->data)->subnodes;
            comps_hslist_insert_at(to_process, x, pair, 0);
        }
        free(current_pair->key);
        free(current_pair);
        free(oldit);
    }

    comps_hslist_destroy(&to_process);
    return ret;
}