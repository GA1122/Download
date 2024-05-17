raptor_rss_promote_namespace_uri(raptor_world *world, raptor_uri* nspace_URI) 
{
   
  if((raptor_uri_equals(nspace_URI,
                           world->rss_namespaces_info_uris[RSS0_9_NS]) ||
      raptor_uri_equals(nspace_URI,
                           world->rss_namespaces_info_uris[RSS1_1_NS]))) {
    nspace_URI = world->rss_namespaces_info_uris[RSS1_0_NS];
  }
  
   
  if(raptor_uri_equals(nspace_URI,
                          world->rss_namespaces_info_uris[ATOM0_3_NS])) {
    nspace_URI = world->rss_namespaces_info_uris[ATOM1_0_NS];
  }

  return nspace_URI;
}