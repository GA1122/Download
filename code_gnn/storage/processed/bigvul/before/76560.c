void nw_buf_pool_release(nw_buf_pool *pool)
{
    for (uint32_t i = 0; i < pool->free; ++i) {
        free(pool->free_arr[i]);
    }
    free(pool->free_arr);
    free(pool);
}
