/*
 * node.h
 * Header for kvs lib nodes
 */

#ifndef _KVS_NODE_H
#define _KVS_NODE_H

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "C" {
#endif // C

// Requires uthash
#include <uthash.h>

typedef unsigned long int Id;
typedef bool BooleanValue;
typedef long long IntegerValue;
typedef long double FloatValue;
typedef bstring StringValue;

// KeyType
typedef enum keytype_t {
	KeyTypeBoolean,
	KeyTypeInteger,
	KeyTypeFloat,
	KeyTypeString
} KeyType;

// KeyTemplate
typedef struct keytemplate_t {
	KeyType type;
	struct keytemplate_t *next;
} KeyTemplate;

// Key
typedef struct key_t {
	Id id;
	char *name;
	KeyType type;					// Type of key
	BooleanValue boolean_value;		// Value store for boolean
	IntegerValue integer_value;		// Value store for integer
	FloatValue float_value;			// Value store for floating
	StringValue string_value;		// Value store for string
	UT_hash_handle hh_id;			// Hash handle for id
	UT_hash_handle hh_name;			// Hash handle for name
} Key;

// Node
typedef struct node_t {
	Id id;
	char *name;
	KeyTemplate *lkeytemplate;		// Key template list
	Key *hkey_id;					// Key id hashtable
	Key *hkey_name;					// Key name hashtable
	struct node_t *pnode;			// Pointer to parent node
	struct node_t *hnode_id;		// Child node id hashtable
	struct node_t *hnode_name;		// Child node name hashtable
	UT_hash_handle hh_id;			// Hash handle for id
	UT_hash_handle hh_name;			// Hash handle for name
} Node;


// Functions

/* Key_new
 * Allocate space for a Key.
 */
Key *Key_new ();

/* Key_destroy
 * De-allocate space for Key.
 */
void Key_destroy (Key *key);

/* KeyTemplate_new
 * Allocate space for a KeyTemplate.
 */
KeyTemplate *KeyTemplate_new ();

/* KeyTemplate_destroy
 * De-allocate space for KeyTemplate.
 */
void KeyTemplate_destroy (KeyTemplate *keytemplate);

/* Node_destroy
 * De-allocate space for Node.
 */
void Node_destroy (Node *node);

/* Node_new
 * Allocate space for a Node.
 */
Node *Node_new ();

/* Node_addKey
 * Add a key to a node.
 */
Key * Node_addKey (Node **node, int id, const char *name);

/* Node_findKeyByName
 * Search for key within node by name.
 */
Key * Node_findKeyByName (Node *node, const char *name);

/* Node_findKeyById
 * Search for key within node by id.
 */
Key * Node_findKeyById (Node *node, const int id);

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif // C

#endif //_KVS_NODE_H
