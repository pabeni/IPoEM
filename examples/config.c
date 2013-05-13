#include <yaml.h>
#include <stdio.h>
#include <glib.h>

void process_layer(yaml_parser_t *parser, GNode *data);
gboolean dump(GNode *n, gpointer data);



int main (int argc, char **argv)
{
    char *file_path;
    yaml_parser_t parser;

    if (argc < 2) {
        printf("Usage: %s <file.yml>\n", argv[0]);
        return 1;
    }

    file_path = argv[1];
    GNode *cfg = g_node_new(file_path);

    FILE *source = fopen(file_path, "rb");
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, source);
    process_layer(&parser, cfg); // Recursive parsing
    yaml_parser_delete(&parser);
    fclose(source);

    printf("Results iteration:\n");
    g_node_traverse(cfg, G_PRE_ORDER, G_TRAVERSE_ALL, -1, dump, NULL);
    g_node_destroy(cfg);

    return(0);
}



enum storage_flags { VAR, VAL, SEQ }; // "Store as" switch

void process_layer(yaml_parser_t *parser, GNode *data)
{
    GNode *last_leaf = data;
    yaml_event_t event;
    int storage = VAR; // mapping cannot start with VAL definition w/o VAR key

    while (1) {
    	yaml_parser_parse(parser, &event);

    	// Parse value either as a new leaf in the mapping
    	//  or as a leaf value (one of them, in case it's a sequence)
    	if (event.type == YAML_SCALAR_EVENT) {
    		if (storage) g_node_append_data(last_leaf, g_strdup((gchar*) event.data.scalar.value));
    		else last_leaf = g_node_append(data, g_node_new(g_strdup((gchar*) event.data.scalar.value)));
    		storage ^= VAL; // Flip VAR/VAL switch for the next event
    	}

    	// Sequence - all the following scalars will be appended to the last_leaf
    	else if (event.type == YAML_SEQUENCE_START_EVENT) storage = SEQ;
    	else if (event.type == YAML_SEQUENCE_END_EVENT) storage = VAR;

    	// depth += 1
    	else if (event.type == YAML_MAPPING_START_EVENT) {
    		process_layer(parser, last_leaf);
    		storage ^= VAL; // Flip VAR/VAL, w/o touching SEQ
    	}

    	// depth -= 1
    	else if (
    		event.type == YAML_MAPPING_END_EVENT
    		|| event.type == YAML_STREAM_END_EVENT
    	) break;

    	yaml_event_delete(&event);
    }
}


gboolean dump(GNode *node, gpointer data)
{
    int i = g_node_depth(node);
    while (--i) printf(" ");
    printf("%s\n", (char*) node->data);
    return(FALSE);
}
