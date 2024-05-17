static int lua_set_cookie(lua_State *L) 
{
    const char *key, *value, *out, *path = "", *domain = "";
    const char *strexpires = "", *strdomain = "", *strpath = "";
    int secure = 0, expires = 0, httponly = 0;
    char cdate[APR_RFC822_DATE_LEN+1];
    apr_status_t rv;
    request_rec *r = ap_lua_check_request_rec(L, 1);
    
     
    if (lua_istable(L, 2)) {
         
         
        lua_pushstring(L, "key");
        lua_gettable(L, -2);
        key = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        
         
        lua_pushstring(L, "value");
        lua_gettable(L, -2);
        value = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        
         
        lua_pushstring(L, "expires");
        lua_gettable(L, -2);
        expires = luaL_optint(L, -1, 0);
        lua_pop(L, 1);
        
         
        lua_pushstring(L, "secure");
        lua_gettable(L, -2);
        if (lua_isboolean(L, -1)) {
            secure = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);
        
         
        lua_pushstring(L, "httponly");
        lua_gettable(L, -2);
        if (lua_isboolean(L, -1)) {
            httponly = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);
        
         
        lua_pushstring(L, "path");
        lua_gettable(L, -2);
        path = luaL_optstring(L, -1, "/");
        lua_pop(L, 1);
        
         
        lua_pushstring(L, "domain");
        lua_gettable(L, -2);
        domain = luaL_optstring(L, -1, "");
        lua_pop(L, 1);        
    }
     
    else {
        key = luaL_checkstring(L, 2);
        value = luaL_checkstring(L, 3);
        secure = 0;
        if (lua_isboolean(L, 4)) {
            secure = lua_toboolean(L, 4);
        }
        expires = luaL_optinteger(L, 5, 0);
    }
    
     
    if (expires > 0) {
        rv = apr_rfc822_date(cdate, apr_time_from_sec(expires));
        if (rv == APR_SUCCESS) {
            strexpires = apr_psprintf(r->pool, "Expires=%s;", cdate);
        }
    }
    
     
    if (path != NULL && strlen(path) > 0) {
        strpath = apr_psprintf(r->pool, "Path=%s;", path);
    }
    
     
    if (domain != NULL && strlen(domain) > 0) {
         
        strdomain = apr_psprintf(r->pool, "Domain=%s;", domain);
    }
    
     
    value = ap_escape_urlencoded(r->pool, value);
    key = ap_escape_urlencoded(r->pool, key);
    
     
    out = apr_psprintf(r->pool, "%s=%s; %s %s %s %s %s", key, value, 
            secure ? "Secure;" : "", 
            expires ? strexpires : "", 
            httponly ? "HttpOnly;" : "", 
            strlen(strdomain) ? strdomain : "", 
            strlen(strpath) ? strpath : "");
    
    apr_table_add(r->err_headers_out, "Set-Cookie", out);
    return 0;
}
