int effect_lib_release(effect_handle_t handle)
{
 effect_context_t *context = (effect_context_t *)handle;
 int status;

 if (lib_init() != 0)
 return init_status;

    ALOGV("%s context %p", __func__, handle);
    pthread_mutex_lock(&lock);
    status = -EINVAL;
 if (effect_exists(context)) {
 output_context_t *out_ctxt = get_output(context->out_handle);
 if (out_ctxt != NULL)
            remove_effect_from_output(out_ctxt, context);
        list_remove(&context->effects_list_node);
 if (context->ops.release)
            context->ops.release(context);
        free(context);
        status = 0;
 }
    pthread_mutex_unlock(&lock);

 return status;
}
