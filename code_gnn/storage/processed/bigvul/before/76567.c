int rpc_decode(nw_ses *ses, void *data, size_t max)
{
    if (max < RPC_PKG_HEAD_SIZE)
        return 0;

    rpc_pkg *pkg = data;
    if (le32toh(pkg->magic) != RPC_PKG_MAGIC)
        return -1;
    uint32_t pkg_size = RPC_PKG_HEAD_SIZE + le16toh(pkg->ext_size) + le32toh(pkg->body_size);
    if (max < pkg_size)
        return 0;

    uint32_t crc32 = le32toh(pkg->crc32);
    pkg->crc32 = 0;
    if (crc32 != generate_crc32c(data, pkg_size))
        return -3;
    pkg->crc32 = crc32;

    pkg->magic     = le32toh(pkg->magic);
    pkg->command   = le32toh(pkg->command);
    pkg->pkg_type  = le16toh(pkg->pkg_type);
    pkg->result    = le32toh(pkg->result);
    pkg->sequence  = le32toh(pkg->sequence);
    pkg->req_id    = le64toh(pkg->req_id);
    pkg->body_size = le32toh(pkg->body_size);
    pkg->ext_size  = le16toh(pkg->ext_size);

    return pkg_size;
}
