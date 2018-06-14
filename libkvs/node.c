/*
 * node.c
 * kvs lib nodes
 * 12.13.2013 - sju
 */

#include <kvs.h>

/*
 * ............ Key
 */

/* Key_new
 * Allocate space for a Key.
 */
Key *Key_new ()
{
	Key *key = (Key *)xalloc(sizeof(Key));

	if (! key) {
		xmsg(mError|mSystem, "failed to allocate Key");
	}

	return key;

} /* Key_new */


/* Key_destroy
 * De-allocate space for Key.
 */
void Key_destroy (Key *key)
{
	if (! key)
		return;

	// Free name
	if (key->name)
		xfree(key->name);

	xfree(key);

} /* Key_destroy */


/*
 * ............ KeyTemplate
 */

/* KeyTemplate_new
 * Allocate space for a KeyTemplate.
 */
KeyTemplate *KeyTemplate_new ()
{
	KeyTemplate *keytemplate = (KeyTemplate *)xalloc(sizeof(KeyTemplate));

	if (! keytemplate) {
		xmsg(mError|mSystem, "failed to allocate KeyTemplate");
	}

	return keytemplate;

} /* KeyTemplate_new */


/* KeyTemplate_destroy
 * De-allocate space for KeyTemplate.
 */
void KeyTemplate_destroy (KeyTemplate *keytemplate)
{
	if (! keytemplate)
		return;

	xfree(keytemplate);

} /* KeyTemplate_destroy */


/*
 * ............ Node
 */

/* Node_new
 * Allocate space for a Node.
 */
Node *Node_new ()
{
	Node *node = (Node *)xalloc(sizeof(Node));

	if (! node) {
		xmsg(mError|mSystem, "failed to allocate Node");
	}

	node->hkey_id = NULL;
	node->hkey_name = NULL;

	return node;

} /* Node_new */


/* Node_destroy
 * De-allocate space for Node.
 */
void Node_destroy (Node *node)
{
	Key *current_key, *tmp_key;

	if (! node)
		return;

	// Free name
	if (node->name)
		xfree(node->name);

	// Delete all key hash by ID
	HASH_ITER(hh_id, node->hkey_id, current_key, tmp_key) {
		HASH_DELETE(hh_id, node->hkey_id, current_key);
	}

	// Delete all key hash by NAME - and destroy keys
	HASH_ITER(hh_name, node->hkey_name, current_key, tmp_key) {
		HASH_DELETE(hh_name, node->hkey_name, current_key);
		Key_destroy(current_key);
	}

	// Free keytemplates
	while (node->lkeytemplate) {
		KeyTemplate *next = node->lkeytemplate->next;

		KeyTemplate_destroy(node->lkeytemplate);

		node->lkeytemplate = next;
	}

	xfree(node);

} /* Node_destroy */


/* Node_addKey
 * Add a key to a node.
 */
Key * Node_addKey (Node **node, int id, const char *name)
{
	Key *key;
	Node *n = *node;

	if (n == NULL) {
		xmsg(mError, "attempt to add key [%s] to a non-existent node", name);
		return (Key *)NULL;
	}

	key = Key_new();
	if (key == NULL) {
		xmsg(mError, "could not create new key [%s]", name);
	}

	// Add id and name
	key->id = id;
	key->name = xstrdup(name);

	// Add to the hashtables
	HASH_ADD(hh_id, n->hkey_id, id, sizeof(int), key);
	HASH_ADD_KEYPTR(hh_name, n->hkey_name, key->name, strlen(key->name), key);

	*node = n;

	return key;

} /* Node_addKey */


/* Node_findKeyByName
 * Search for key within node by name.
 */
Key * Node_findKeyByName (Node *node, const char *name)
{
	Key *key;

	if (node == NULL) {
		xmsg(mError, "attempt to find key [%s] in a non-existent node", name);
		return (Key *)NULL;
	}

	HASH_FIND(hh_name, node->hkey_name, name, strlen(name), key);

	return key;

} /* Node_findKeyByName */


/* Node_findKeyById
 * Search for key within node by id.
 */
Key * Node_findKeyById (Node *node, const int id)
{
	Key *key;

	if (node == NULL) {
		xmsg(mError, "attempt to find key [%d] in a non-existent node", id);
		return (Key *)NULL;
	}

	HASH_FIND(hh_id, node->hkey_id, &id, sizeof (int), key);

	return key;

} /* Node_findKeyById */
