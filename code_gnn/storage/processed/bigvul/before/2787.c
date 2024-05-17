gs_getdefaultlibdevice(gs_memory_t *mem)
{
    const gx_device *const *list;
    int count = gs_lib_device_list(&list, NULL);
    const char *name, *end, *fin;
    int i;

     
     
    if (mem && mem->gs_lib_ctx && mem->gs_lib_ctx->default_device_list) {
        name = mem->gs_lib_ctx->default_device_list;
        fin = name + strlen(name);
    }
    else {
        name = gs_dev_defaults;
        fin = name + strlen(name);
    }

     
    while (name < fin) {

       
      while ((name < fin) && (*name == ' ' || *name == '\t'))
        name++;
      end = name;
      while ((end < fin) && (*end != ' ') && (*end != '\t'))
        end++;

       
      for (i = 0; i < count; i++)
        if ((end - name) == strlen(list[i]->dname))
          if (!memcmp(name, list[i]->dname, end - name))
            return gs_getdevice(i);

       
      name = end;
    }

     
    return gs_getdevice(0);
}
