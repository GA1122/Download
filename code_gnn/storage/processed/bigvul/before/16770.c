static int qcow2_downgrade(BlockDriverState *bs, int target_version)
{
    BDRVQcowState *s = bs->opaque;
    int current_version = s->qcow_version;
    int ret;

    if (target_version == current_version) {
        return 0;
    } else if (target_version > current_version) {
        return -EINVAL;
    } else if (target_version != 2) {
        return -EINVAL;
    }

    if (s->refcount_order != 4) {
         
        error_report("qcow2_downgrade: Image refcount orders other than 4 are "
                     "currently not supported.");
        return -ENOTSUP;
    }

     
    if (s->incompatible_features & QCOW2_INCOMPAT_DIRTY) {
        ret = qcow2_mark_clean(bs);
        if (ret < 0) {
            return ret;
        }
    }

     

    if (s->incompatible_features) {
        return -ENOTSUP;
    }

     
    s->compatible_features = 0;
     

     
    s->autoclear_features = 0;

    ret = qcow2_expand_zero_clusters(bs);
    if (ret < 0) {
        return ret;
    }

    s->qcow_version = target_version;
    ret = qcow2_update_header(bs);
    if (ret < 0) {
        s->qcow_version = current_version;
        return ret;
    }
    return 0;
}
