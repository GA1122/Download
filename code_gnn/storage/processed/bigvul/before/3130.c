static int septransform(i_ctx_t *i_ctx_p, ref *sepspace, int *usealternate, int *stage, int *stack_depth)
{
    gx_device * dev = igs->device;
    ref sname, proc;
    int code, colorant_number;

    code = array_get(imemory, sepspace, 1, &sname);
    if (code < 0)
        return code;
    if (r_has_type(&sname, t_name)) {
        name_string_ref(imemory, &sname, &sname);
    }

     
    if (r_size(&sname) == 3 &&
        strncmp("All", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
        *usealternate = 0;
        return 0;
    }
    if (r_size(&sname) == 4 &&
        strncmp("None", (const char *)sname.value.bytes, r_size(&sname)) == 0) {
        *usealternate = 0;
        return 0;
    }
     
    colorant_number = (*dev_proc(dev, get_color_comp_index))
                (dev, (const char *)sname.value.bytes, r_size(&sname), SEPARATION_NAME);
    if (colorant_number >= 0) {		 
        *usealternate = 0;
    } else
        *usealternate = 1;

    if (*usealternate && *stage == 0) {
        (*stage)++;
        esp++;
        code = array_get(imemory, sepspace, 3, &proc);
        if (code < 0)
            return code;
        *esp = proc;
        return o_push_estack;
    }
    *stage = 0;
    return 0;
}
