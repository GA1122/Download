bool effects_enabled()
{
 struct listnode *out_node;

    list_for_each(out_node, &active_outputs_list) {
 struct listnode *fx_node;
 output_context_t *out_ctxt = node_to_item(out_node,
 output_context_t,
                                                  outputs_list_node);

        list_for_each(fx_node, &out_ctxt->effects_list) {
 effect_context_t *fx_ctxt = node_to_item(fx_node,
 effect_context_t,
                                                     output_node);
 if ((fx_ctxt->state == EFFECT_STATE_ACTIVE) &&
 (fx_ctxt->ops.process != NULL))
 return true;
 }
 }
 return false;
}
