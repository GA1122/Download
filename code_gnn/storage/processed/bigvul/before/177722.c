int equalizer_start(effect_context_t *context, output_context_t *output)
{
 equalizer_context_t *eq_ctxt = (equalizer_context_t *)context;

    ALOGV("%s: %p", __func__, output->ctl);
    eq_ctxt->ctl = output->ctl;
 if (offload_eq_get_enable_flag(&(eq_ctxt->offload_eq)))
 if (eq_ctxt->ctl)
            offload_eq_send_params(eq_ctxt->ctl, &eq_ctxt->offload_eq,
                                   OFFLOAD_SEND_EQ_ENABLE_FLAG |
                                   OFFLOAD_SEND_EQ_BANDS_LEVEL);
 return 0;
}
