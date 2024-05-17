xmlFreeURI(xmlURIPtr uri) {
 if (uri == NULL) return;

 if (uri->scheme != NULL) xmlFree(uri->scheme);
 if (uri->server != NULL) xmlFree(uri->server);
 if (uri->user != NULL) xmlFree(uri->user);
 if (uri->path != NULL) xmlFree(uri->path);
 if (uri->fragment != NULL) xmlFree(uri->fragment);
 if (uri->opaque != NULL) xmlFree(uri->opaque);
 if (uri->authority != NULL) xmlFree(uri->authority);
 if (uri->query != NULL) xmlFree(uri->query);
 if (uri->query_raw != NULL) xmlFree(uri->query_raw);
    xmlFree(uri);
}
