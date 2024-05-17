static int devicentransform(i_ctx_t *i_ctx_p, ref *devicenspace, int *usealternate, int *stage, int *stack_depth)
{
    gx_device * dev = igs->device;
    ref narray, sname, proc;
    int i, code, colorant_number;

    *usealternate = 0;
    code = array_get(imemory, devicenspace, 1, &narray);
    if (code < 0)
        return code;
    if (!r_is_array(&narray))
        return_error(gs_error_typecheck);

    for (i=0;i<r_size(&narray);i++) {
        code = array_get(imemory, &narray, i, &sname);
        if (code < 0)
            return code;
        if (r_has_type(&sname, t_name)) {
            name_string_ref(imemory, &sname, &sname);
        }

         
        if (r_size(&sname) == 3 &&
            strncmp("All", (const char *)sname.value.bytes, r_size(&sname)) == 0)
            continue;
        if (r_size(&sname) == 4 &&
            strncmp("None", (const char *)sname.value.bytes, r_size(&sname)) == 0)
            continue;
         
        colorant_number = (*dev_proc(dev, get_color_comp_index))
                (dev, (const char *)sname.value.bytes, r_size(&sname), SEPARATION_NAME);
        if (colorant_number < 0) {		 
            *usealternate = 1;
            break;
        }
    }
    if (*usealternate && *stage == 0) {
        (*stage)++;
        esp++;
        code = array_get(imemory, devicenspace, 3, &proc);
        if (code < 0)
            return code;
        *esp = proc;
        return o_push_estack;
    }

    if (*stage == 1){
        *stack_depth = 0;
        *stage = 0;
    }
    return 0;
}
