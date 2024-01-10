#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>


#define COMMAND_SIZE 20
#define KEY_SIZE 50
#define VALUE_SIZE 100
#define INPUT_SIZE 100
#define MEMBER_SIZE 100
#define INITIAL_TABLE_SIZE 10
#define FIELD_SIZE 50
#define LOAD_FACTOR_THRESHOLD 0.9
#define MIN_LOAD_FACTOR_THRESHOLD 0.2


// KEY-VALUE
typedef struct KeyValue {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];   
} KeyValue;

typedef struct Database {
    KeyValue* data;     //存一堆KeyValue的指標
    int size;
} Database;

Database* createDatabase();

//創建 inset
void insert(Database* db, const char* key, const char* value);

//不會顯示Successful的創建
void pre_insert(Database* db, const char* key, const char* value);

//讀取 read
void read(Database* db, const char* key);

//不會printf的讀取
void read_notprint(Database* db, const char* key);

//更新 update
void update(Database* db, const char* key, const char* value);


//刪除 delete
void del(Database* db, const char* key);



//LIST
typedef struct NodeForList
{
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct NodeForList* prev;
    struct NodeForList* next;
} NodeForList;

NodeForList *createNode(const char* key, const char* value);

// 加入 PUSH
void insertRight_node(NodeForList** head, NodeForList** bottom, const char* key, const char* value);

void insertLeft_node(NodeForList** head, NodeForList** bottom, const char* key, const char* value);

// 刪除 POP
void lpop(NodeForList** head, NodeForList** bottom, const char* key, int num);

void rpop(NodeForList** head, NodeForList** bottom, const char* key, int num);

// 長度 llen
void llen(NodeForList* head, const char* key);

// 列印範圍 lrange
void lrange(NodeForList* head, NodeForList* bottom, const char* key, int start, int stop);

//以備不時之需
// 讀取 read
void read_node(NodeForList* head, const char* key);

// 更新 update
void update_node(NodeForList* head, const char* key, const char* value);

// 刪除 delete
void del_node(NodeForList** head, NodeForList** bottom, const char* key);

void freeList(NodeForList* head);


//TREE
typedef struct TreeNode {
    char key[KEY_SIZE];
    struct TreeNode** value;
    int numValues;
} TreeNode;

typedef struct TreeDatabase{
    TreeNode** tree;
    int numOfTree;
} TreeDatabase;

TreeDatabase* createTreeDatabase();
TreeNode* createTreeNode(const char* key);
void makeATree(TreeDatabase* db, const char* key);
void addTreeNode_1(TreeDatabase* db, const char* treeName, const char* value);
void addTreeNodeRecursive(TreeNode* parent, const char* nodeName, const char* value);
void addTreeNode_2(TreeDatabase* db, const char* treeName, const char* nodeName, const char* value);
void printTreeRecursive(TreeNode* node, int numOfSpace);
void printTree(TreeDatabase* db, const char* treeName);
TreeNode* findTreeNodeRecursive(TreeNode* root, const char* nodeName);
void findTreeNode(TreeDatabase* db, const char* treeName,  const char* nodeName);
void freeNode(TreeNode* node);
void deleteNodeRecursive(TreeNode* root, const char* nodeName);
void deleteTreeNode(TreeDatabase* db, const char* treeName, const char* nodeName);
void freeTreeDatabase(TreeDatabase* db);


// SET
typedef struct MEMBER{
    int score;
    char member[MEMBER_SIZE];
    struct MEMBER *next;
} MEMBER;

typedef struct KeyofSet{
    char key[KEY_SIZE];
    MEMBER *members;    //指向一個Linked List
} KeyofSet;


typedef struct DatabaseForSet { //有很多key的database
    KeyofSet **sets;    //指標陣列
    int numSets;     
} DatabaseForSet;

DatabaseForSet* createDatabaseForSet();

MEMBER *createMember(const int score, const char *member);


void addAMember(KeyofSet *set, const char *key, const int score, const char *member);

void ZADD(DatabaseForSet* db, const char *key, const int score, const char *member);

void ZCARD(DatabaseForSet *db, const char *key);

void ZCOUNT(DatabaseForSet* db, const char *key, const int min, const int max);

void ZINTERSTORE(DatabaseForSet* db, const char *newkey, const char *key1, const char *key2);

void ZUNIONSTORE(DatabaseForSet* db, const char *newkey, const char *key1, const char *key2);

void ZRANGE(DatabaseForSet* db, const char *key, int start, int stop);

void ZRANGEBYSCORE(DatabaseForSet* db, const char *key, const int min, const int max);

void ZRANK(DatabaseForSet* db, const char *key, const char *member);

void ZREM(DatabaseForSet* db, const char *key, const char *member);

void ZREMRANGEBYLEX(DatabaseForSet* db, const char *key, char *min, char *max);

void ZREMRANGEBYRANK(DatabaseForSet* db, const char *key, const int start, const int stop);

void ZREMRANGEBYSCORE(DatabaseForSet* db, const char *key, const int min, const int max);


//HASH
typedef struct NodeForHash {
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct NodeForHash *next;
} NodeForHash;

// 定義雜湊表
typedef struct HashTable {
    NodeForHash **table;
    char field[FIELD_SIZE];
    int size;
    int table_size;
} HashTable;

typedef struct DatabaseForHash {
    HashTable **hashTables;  // 指向哈希表陣列的指標
    int numHashTables;        // 哈希表的數量
} DatabaseForHash;

NodeForHash *createNodeForHash(const char *key, const char *value);

void initHashTable(HashTable *hashTable, const char *field, int table_size);

DatabaseForHash* createDatabaseForHash();

HashTable* searchForField(DatabaseForHash* db, const char *field);

int hashFunction(const char *key, const int table_size);

void insertItemForHash(HashTable *hashTable, const char *key, const char *value);

void insertItemForHash_withoutLF(HashTable *hashTable, const char *key, const char *value);

void checkLoadFactor(HashTable *hashTable);

void resizeHashTable(HashTable *ht, int newSize);

void freeHashTable(HashTable *hashTable);

NodeForHash* searchHashNode(const HashTable *hashTable, const char *key);

void HSET(DatabaseForHash *db, const char *key, const char *field, const char *value);

void HGET(DatabaseForHash *db, const char *key, const char *field);

void HDEL(DatabaseForHash *db, const char *key, const char *field, const int isEXPIRE);

void freeHashDatabase(DatabaseForHash *db);

typedef struct EXPIRE_input {
    DatabaseForHash *db;
    char key[KEY_SIZE];
    int time;
} EXPIRE_input;

void EXPIRE(DatabaseForHash *db, const char* key, const int time);
DWORD WINAPI countDownToDEL(LPVOID lpParam);
