PP_Instance ResourceTracker::GetInstanceForResource(PP_Resource pp_resource) {
  scoped_refptr<Resource> resource(GetResource(pp_resource));
  if (!resource.get())
    return 0;
  return resource->instance()->pp_instance();
}
