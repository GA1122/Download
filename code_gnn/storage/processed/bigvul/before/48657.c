int h2_session_push_enabled(h2_session *session)
{
     
    return (session->remote.accepting  
            && h2_config_geti(session->config, H2_CONF_PUSH)
            && nghttp2_session_get_remote_settings(session->ngh2, 
                   NGHTTP2_SETTINGS_ENABLE_PUSH));
}
