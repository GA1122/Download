static void release_memory_resource(struct resource *resource)
{
	if (!resource)
		return;

	 
	release_resource(resource);
	kfree(resource);
}
