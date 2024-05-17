int qcow2_alloc_clusters_at(BlockDriverState *bs, uint64_t offset,
    int nb_clusters)
{
    BDRVQcowState *s = bs->opaque;
    uint64_t cluster_index;
    uint64_t i;
    int refcount, ret;

    assert(nb_clusters >= 0);
    if (nb_clusters == 0) {
        return 0;
    }

    do {
         
        cluster_index = offset >> s->cluster_bits;
        for(i = 0; i < nb_clusters; i++) {
            refcount = get_refcount(bs, cluster_index++);

            if (refcount < 0) {
                return refcount;
            } else if (refcount != 0) {
                break;
            }
        }

         
        ret = update_refcount(bs, offset, i << s->cluster_bits, 1,
                              QCOW2_DISCARD_NEVER);
    } while (ret == -EAGAIN);

    if (ret < 0) {
        return ret;
    }

    return i;
}