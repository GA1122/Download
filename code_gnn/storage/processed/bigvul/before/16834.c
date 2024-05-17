static int validate_table_offset(BlockDriverState *bs, uint64_t offset,
                                 uint64_t entries, size_t entry_len)
{
    BDRVQcowState *s = bs->opaque;
    uint64_t size;

     
    if (entries > INT64_MAX / entry_len) {
        return -EINVAL;
    }

    size = entries * entry_len;

    if (INT64_MAX - size < offset) {
        return -EINVAL;
    }

     
    if (offset & (s->cluster_size - 1)) {
        return -EINVAL;
    }

    return 0;
}
