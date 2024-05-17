nw_buf_pool *nw_buf_pool_create(uint32_t size)
{
    nw_buf_pool *pool = malloc(sizeof(nw_buf_pool));
    if (pool == NULL)
        return NULL;

    pool->size = size;
    pool->used = 0;
    pool->free = 0;
    pool->free_total = NW_BUF_POOL_INIT_SIZE;
    pool->free_arr = malloc(pool->free_total * sizeof(nw_buf *));
    if (pool->free_arr == NULL) {
        free(pool);
        return NULL;
    }

    return pool;
}
