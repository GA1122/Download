void remove_effect_from_output(output_context_t * output,
 effect_context_t *context)
{
 struct listnode *fx_node;

    list_for_each(fx_node, &output->effects_list) {
 effect_context_t *fx_ctxt = node_to_item(fx_node,
 effect_context_t,
                                                 output_node);
 if (fx_ctxt == context) {
 if (context->ops.stop)
                context->ops.stop(context, output);
            list_remove(&context->output_node);
 return;
 }
 }
}
