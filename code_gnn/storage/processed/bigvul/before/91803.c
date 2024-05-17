char comps_objrtree_paircmp(void *obj1, void *obj2) {
    if (strcmp(((COMPS_ObjRTreePair*)obj1)->key,
               ((COMPS_ObjRTreePair*)obj2)->key) != 0)
        return 0;
    return comps_object_cmp(((COMPS_ObjRTreePair*)obj1)->data,
                            ((COMPS_ObjRTreePair*)obj2)->data);
}
