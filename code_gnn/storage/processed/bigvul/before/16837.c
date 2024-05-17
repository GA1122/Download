static int bdrv_assign_node_name(BlockDriverState *bs,
                                 const char *node_name,
                                 Error **errp)
{
    if (!node_name) {
        return 0;
    }

     
    if (node_name[0] == '\0') {
        error_setg(errp, "Empty node name");
        return -EINVAL;
    }

     
    if (bdrv_find(node_name)) {
        error_setg(errp, "node-name=%s is conflicting with a device id",
                   node_name);
        return -EINVAL;
    }

     
    if (bdrv_find_node(node_name)) {
        error_setg(errp, "Duplicate node name");
        return -EINVAL;
    }

     
    pstrcpy(bs->node_name, sizeof(bs->node_name), node_name);
    QTAILQ_INSERT_TAIL(&graph_bdrv_states, bs, node_list);

    return 0;
}
