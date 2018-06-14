/*
 * test_kvs_node.c
 * kvs lib node tests
 * 12.13.2013 - sju
 */

#include "kvs.h"
#include "tests.h"
#include "test_kvs_node.h"

static Node * create_node_with_testvalues()
{
	Node *node = Node_new();
	int id = 0;
	const char **name,
		*names[] = {
				"RobMariano",
				"RobCesternino",
				"ParvatiShallow",
				"TysonApostol",
				"AmandaKimmel",
				"RandyBailey",
				"TomWestman",
				"CirieFields",
				NULL
		};

	for (name = names; *name != NULL; name++)
		if (Node_addKey(&node, id++, *name) == (Key *)NULL)
			return false;

	return node;
}

TEST(node_new)
{
	Node *node = Node_new();

	if (node == NULL)
		TEST_FAIL("could not create node");

	TEST_PASS;
}

TEST(key_new)
{
	Key *key = Key_new();

	if (key == NULL)
		TEST_FAIL("could not create key");

	TEST_PASS;
}

TEST(keytemplate_new)
{
	KeyTemplate *keytemplate = KeyTemplate_new();

	if (keytemplate == NULL)
		TEST_FAIL("could not create keytemplate");

	TEST_PASS;
}

TEST(node_destroy)
{
	Node *node = Node_new();

	Node_destroy(node);

	TEST_PASS;
}

TEST(node_add_key)
{
	Node *node = Node_new();
	Key *key;

	key = Node_addKey(&node, 1, "Foo");
	if (! key)
		TEST_FAIL("could not add key");

	TEST_PASS;
}

TEST(key_destroy)
{
	Key *key = Key_new();

	Key_destroy(key);

	TEST_PASS;
}

TEST(keytemplate_destroy)
{
	KeyTemplate *keytemplate = KeyTemplate_new();

	KeyTemplate_destroy(keytemplate);

	TEST_PASS;
}

TEST(node_find_key_by_name)
{
	Node *node;
	Key *key_find;
	const char *searchstr = "CirieFields";

	if ((node = create_node_with_testvalues()) == (Node *)NULL)
		TEST_FAIL("could not add node test values");

	key_find = Node_findKeyByName(node, searchstr);
	if (! key_find)
		TEST_FAIL("could not find key");

	Node_destroy(node);

	TEST_PASS;
}

TEST(node_do_not_find_key_by_name)
{
	Node *node = Node_new();
	Key *key_add, *key_find;
	const char *searchstr = "BobBarker";

	if ((node = create_node_with_testvalues()) == (Node *)NULL)
		TEST_FAIL("could not add node test values");

	key_find = Node_findKeyByName(node, searchstr);
	if (key_find != NULL)
		TEST_FAIL("key should not exist");

	Node_destroy(node);

	TEST_PASS;
}

TEST(node_find_key_by_id)
{
	Node *node;
	Key *key_find;
	const int searchnum = 4;

	if ((node = create_node_with_testvalues()) == (Node *)NULL)
		TEST_FAIL("could not add node test values");

	key_find = Node_findKeyById(node, searchnum);
	if (! key_find)
		TEST_FAIL("could not find key id");

	Node_destroy(node);

	TEST_PASS;
}

TEST(node_do_not_find_key_by_id)
{
	Node *node = Node_new();
	Key *key_add, *key_find;
	const int searchnum = 9999;

	if ((node = create_node_with_testvalues()) == (Node *)NULL)
		TEST_FAIL("could not add node test values");

	key_find = Node_findKeyById(node, searchnum);
	if (key_find != NULL)
		TEST_FAIL("key should not exist");

	Node_destroy(node);

	TEST_PASS;
}

TEST(node_memory_leak)
{
	unsigned long cycles = 99999999L;

	while (--cycles) {
		Node *node;
		Key *key_find;
		const int searchnum = 4;

		if ((node = create_node_with_testvalues()) == (Node *)NULL)
			TEST_FAIL("could not add node test values");

		key_find = Node_findKeyById(node, searchnum);
		if (! key_find)
			TEST_FAIL("could not find key id");

		Node_destroy(node);
	}

	TEST_PASS;
}


/* test_kvs_node
 * Main entry point for node tests
 */
TEST_UNIT test_kvs_node()
{
	BEGIN_TEST_UNIT("kvs lib node tests");

	// Normal tests
	RUN_TEST(node_new);
	RUN_TEST(key_new);
	RUN_TEST(keytemplate_new);
	RUN_TEST(node_destroy);
	RUN_TEST(key_destroy);
	RUN_TEST(keytemplate_destroy);
	RUN_TEST(node_add_key);
	RUN_TEST(node_find_key_by_name);
	RUN_TEST(node_do_not_find_key_by_name);
	RUN_TEST(node_find_key_by_id);
	RUN_TEST(node_do_not_find_key_by_id);

	// Special tests
	//RUN_TEST(node_memory_leak);

	END_TEST_UNIT();

} /* test_kvs_node */
