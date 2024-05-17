output_context_t *get_output(audio_io_handle_t output)
{
 struct listnode *node;

    list_for_each(node, &active_outputs_list) {
 output_context_t *out_ctxt = node_to_item(node,
 output_context_t,
                                                  outputs_list_node);
 if (out_ctxt->handle == output)
 return out_ctxt;
 }
 return NULL;
}
