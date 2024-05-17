signed char comps_objmrtree_cmp(COMPS_ObjMRTree *ort1, COMPS_ObjMRTree *ort2) {
    COMPS_HSList *values1, *values2;
    COMPS_HSListItem *it;
    COMPS_Set *set1, *set2;
    signed char ret;
    values1 = comps_objmrtree_pairs(ort1);
    values2 = comps_objmrtree_pairs(ort2);
    set1 = comps_set_create();
    comps_set_init(set1, NULL, NULL, NULL, &comps_objmrtree_paircmp);
    set2 = comps_set_create();
    comps_set_init(set2, NULL, NULL, NULL, &comps_objmrtree_paircmp);
    for (it = values1->first; it != NULL; it = it->next) {
        comps_set_add(set1, it->data);
    }
    for (it = values2->first; it != NULL; it = it->next) {
        comps_set_add(set2, it->data);
    }

    ret = comps_set_cmp(set1, set2);
    comps_set_destroy(&set1);
    comps_set_destroy(&set2);

    comps_hslist_destroy(&values1);
    comps_hslist_destroy(&values2);
    return !ret;
}