static apr_status_t lua_input_filter_handle(ap_filter_t *f,
                                       apr_bucket_brigade *pbbOut,
                                       ap_input_mode_t eMode,
                                       apr_read_type_e eBlock,
                                       apr_off_t nBytes) 
{
    request_rec *r = f->r;
    int rc, lastCall = 0;
    lua_State *L;
    lua_filter_ctx* ctx;
    conn_rec *c = r->connection;
    apr_status_t ret;
    
     
    if (!f->ctx) {
        rc = lua_setup_filter_ctx(f,r,&ctx);
        f->ctx = ctx;
        if (rc == APR_EGENERAL) {
            ctx->broken = 1;
            ap_remove_input_filter(f); 
            return HTTP_INTERNAL_SERVER_ERROR;
        }
        if (rc == APR_ENOENT ) {
            ap_remove_input_filter(f);
            ctx->broken = 1;
        }
        if (rc == APR_SUCCESS) {
            ctx->tmpBucket = apr_brigade_create(r->pool, c->bucket_alloc);
        }
    }
    ctx = (lua_filter_ctx*) f->ctx;
    L = ctx->L;
     
    if (ctx->broken) {
        return ap_get_brigade(f->next, pbbOut, eMode, eBlock, nBytes);
    }
    
    if (APR_BRIGADE_EMPTY(ctx->tmpBucket)) {
        ret = ap_get_brigade(f->next, ctx->tmpBucket, eMode, eBlock, nBytes);
        if (eMode == AP_MODE_EATCRLF || ret != APR_SUCCESS)
            return ret;
    }
    
     
    if (!ctx->broken) {
        lastCall = 0;
        while(!APR_BRIGADE_EMPTY(ctx->tmpBucket)) {
            apr_bucket *pbktIn = APR_BRIGADE_FIRST(ctx->tmpBucket);
            apr_bucket *pbktOut;
            const char *data;
            apr_size_t len;
            
            if (APR_BUCKET_IS_EOS(pbktIn)) {
                APR_BUCKET_REMOVE(pbktIn);
                break;
            }

             
            ret = apr_bucket_read(pbktIn, &data, &len, eBlock);
            if (ret != APR_SUCCESS)
                return ret;

             
            lastCall++;
            lua_pushlstring(L, data, len);
            lua_setglobal(L, "bucket");
            
             
            if (lua_resume(L, 0) == LUA_YIELD) {
                size_t olen;
                const char* output = lua_tolstring(L, 1, &olen);
                pbktOut = apr_bucket_heap_create(output, olen, 0, c->bucket_alloc);
                APR_BRIGADE_INSERT_TAIL(pbbOut, pbktOut);
                apr_bucket_delete(pbktIn);
                return APR_SUCCESS;
            }
            else {
                ctx->broken = 1;
                ap_lua_release_state(L, ctx->spec, r);
                ap_remove_input_filter(f); 
                apr_bucket_delete(pbktIn);
                return HTTP_INTERNAL_SERVER_ERROR;
            }
        }
         
        if (lastCall == 0) {
            apr_bucket *pbktEOS = apr_bucket_eos_create(c->bucket_alloc);
            lua_pushnil(L);
            lua_setglobal(L, "bucket");
            if (lua_resume(L, 0) == LUA_YIELD) {
                apr_bucket *pbktOut;
                size_t olen;
                const char* output = lua_tolstring(L, 1, &olen);
                pbktOut = apr_bucket_heap_create(output, olen, 0, c->bucket_alloc);
                APR_BRIGADE_INSERT_TAIL(pbbOut, pbktOut);
            }
            APR_BRIGADE_INSERT_TAIL(pbbOut,pbktEOS);
            ap_lua_release_state(L, ctx->spec, r);
        }
    }
    return APR_SUCCESS;
}
