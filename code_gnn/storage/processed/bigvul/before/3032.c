static int checkGamma(i_ctx_t * i_ctx_p, ref *CIEdict, int numvalues)
{
    int code = 0, i;
    float value[3];
    ref *tempref, valref;

    code = dict_find_string(CIEdict, "Gamma", &tempref);
    if (code > 0 && !r_has_type(tempref, t_null)) {
        if (numvalues > 1) {
             
            if (!r_is_array(tempref))
                return_error(gs_error_typecheck);
            if (r_size(tempref) != numvalues)
                return_error(gs_error_rangecheck);
            for (i=0;i<numvalues;i++) {
                code = array_get(imemory, tempref, i, &valref);
                if (code < 0)
                    return code;
                if (r_has_type(&valref, t_integer))
                    value[i] = (float)valref.value.intval;
                else if (r_has_type(&valref, t_real))
                    value[i] = (float)valref.value.realval;
                else
                    return_error(gs_error_typecheck);
                if (value[i] <= 0) return_error(gs_error_rangecheck);
            }
        } else {
             
            if (r_has_type(tempref, t_real))
                value[0] = (float)(tempref->value.realval);
            else if (r_has_type(tempref, t_integer))
                    value[0] = (float)(tempref->value.intval);
            else
                return_error(gs_error_typecheck);
            if (value[0] <= 0) return_error(gs_error_rangecheck);
        }
    }
    return 0;
}
