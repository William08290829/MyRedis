#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>
#include "mylib.h"


#define COMMAND_SIZE 20
#define KEY_SIZE 50
#define VALUE_SIZE 100
#define INPUT_SIZE 100
#define MEMBER_SIZE 100
#define INITIAL_TABLE_SIZE 10
#define FIELD_SIZE 50
#define LOAD_FACTOR_THRESHOLD 0.9
#define MIN_LOAD_FACTOR_THRESHOLD 0.2


Database* createDatabase() {
    Database* db = (Database*)malloc(sizeof(Database));
    db->size = 0;
    db->data = NULL;
    return db;
}

//創建 inset
void insert(Database* db, const char* key, const char* value) {
    db->data = (KeyValue*)realloc(db->data, sizeof(KeyValue) * (size_t)(db->size + 1));
    
    strcpy(db->data[db->size].key, key);
    strcpy(db->data[db->size].value, value);
    db->size++;

    printf("Insert [%s] Successfully.\n", key);
}

//不會顯示Successful的創建
void pre_insert(Database* db, const char* key, const char* value) {
    db->data = (KeyValue*)realloc(db->data, sizeof(KeyValue) * (size_t)(db->size + 1));
    
    strcpy(db->data[db->size].key, key);
    strcpy(db->data[db->size].value, value);
    db->size++;
}

//讀取 read
void read(Database* db, const char* key) {
    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->data[i].key, key) == 0) {
            printf("{\n");
            printf("    Key: %s\n", db->data[i].key);
            printf("    Value: %s\n", db->data[i].value);
            printf("}\n");
            return;
        }
    }
    printf("Not Found [%s] in Database.\n", key);
}

//不會printf的讀取
void read_notprint(Database* db, const char* key) {
    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->data[i].key, key) == 0) {
            return;
        }
    }
    printf("Not Found [%s] in Database.\n", key);
}

//更新 update
void update(Database* db, const char* key, const char* value) {
    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->data[i].key, key) == 0) {    
            strcpy(db->data[i].value, value);
            printf("Update [%s] Successfully\n", key);
            return;
        }
    }
    printf("[%s] Not Found. Cannot update.\n", key);
}


//刪除 delete
void del(Database* db, const char* key) {
    for (int i = 0; i < db->size; i++) {
        if (strcmp(db->data[i].key, key) == 0) {
            // 將後面的資料往前移動
            for (int j = i; j < db->size - 1; j++) {
                strcpy(db->data[j].key, db->data[j + 1].key);
                strcpy(db->data[j].value, db->data[j + 1].value);
            }
            db->size--;

            printf("Deleted [%s] Successfully.\n", key);
            return;
        }
    }
    printf("Key not found. Cannot remove.\n");
}



//LIST
NodeForList *createNode(const char* key, const char* value)
{
    NodeForList *newNode = (NodeForList *)malloc(sizeof(NodeForList));
    if (newNode == NULL)
    {
        perror("Memory allocation failed");
        exit(1);
    }

    strncpy(newNode->key, key, KEY_SIZE);
    strncpy(newNode->value, value, VALUE_SIZE);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// 加入 PUSH
void insertRight_node(NodeForList** head, NodeForList** bottom, const char* key, const char* value)
{
    NodeForList* newNode = createNode(key, value);

    if (*head == NULL)
    {
        *head = newNode;
        *bottom = newNode;
    }
    else
    {
        (*bottom)->next = newNode;
        newNode->prev = *bottom;
        *bottom = newNode;
    }

    printf("Insert [%s] in [%s] Successfully.\n", value,key);
}

void insertLeft_node(NodeForList** head, NodeForList** bottom, const char* key, const char* value)
{
    NodeForList* newNode = createNode(key, value);

    if (*head == NULL)
    {
        *head = newNode;
        *bottom = newNode;
    }
    else
    {
        (*head)->prev = newNode;
        newNode->next = *head;
        *head = newNode;
    }

    printf("Insert [%s] in [%s] Successfully.\n", value,key);
}

// 刪除 POP
void lpop(NodeForList** head, NodeForList** bottom, const char* key, int num)
{
    if(num == 0){
        num = 1;
    }

    if (*head == NULL)
    {
        printf("Database is Empty. Cannot delete Value.\n");
        return;
    }
    else
    {
        NodeForList* Pointer = *head;
        int counter = 0;
        while (Pointer != NULL && counter < num)
        {
            if (strcmp(Pointer->key, key) == 0)
            {
                counter++;

                if (Pointer == *head) //刪除第一個
                {
                    *head = Pointer->next;
                    if (*head != NULL) {
                        (*head)->prev = NULL;
                    }
                }
                else if (Pointer == *bottom) //刪除最後一個
                {
                    *bottom = Pointer->prev;
                    if (*bottom != NULL) {
                        (*bottom)->next = NULL;
                    }
                }
                else
                {
                    if (Pointer->prev != NULL)
                    {
                        Pointer->prev->next = Pointer->next; //Pointer的前一個指向下一個
                    }
                    if (Pointer->next != NULL)
                    {
                        Pointer->next->prev = Pointer->prev; //Pointer的下一個指向前一個
                    }
                }

                NodeForList* tmp = Pointer;
                Pointer = Pointer->next;
                tmp->prev = NULL;
                tmp->next = NULL;
                printf("Delete [%s] in [%s] Successfully.\n", tmp->value, key);
                free(tmp);
            }
            else{
                Pointer = Pointer->next;
            }
        }

        if (counter == 0)
        {
            printf("Key not found. Cannot delete.\n");
            return;
        }
        else
        {
            return;
        }
    }
}

void rpop(NodeForList** head, NodeForList** bottom, const char* key, int num)
{
    if(num == 0){
        num = 1;
    }

    if (*head == NULL)
    {
        printf("Database is Empty. Cannot delete Value.\n");
        return;
    }
    else
    {
        NodeForList* Pointer = *bottom;
        int counter = 0;
        while (Pointer != NULL && counter < num)
        {
            if (strcmp(Pointer->key, key) == 0)
            {
                // printf("counter: %d\n", counter);
                counter++;

                if (Pointer == *head) //刪除第一個
                {
                    *head = Pointer->next;
                    if (*head != NULL) {
                        (*head)->prev = NULL;
                    }
                }
                else if (Pointer == *bottom) //刪除最後一個
                {
                    *bottom = Pointer->prev;
                    if (*bottom != NULL) {
                        (*bottom)->next = NULL;
                    }
                }
                else
                {
                    if (Pointer->prev != NULL)
                    {
                        Pointer->prev->next = Pointer->next; //Pointer的前一個指向下一個
                    }
                    if (Pointer->next != NULL)
                    {
                        Pointer->next->prev = Pointer->prev; //Pointer的下一個指向前一個
                    }
                }

                NodeForList* tmp = Pointer;
                Pointer = Pointer->prev;
                tmp->prev = NULL;
                tmp->next = NULL;
                printf("Delete [%s] in [%s] Successfully.\n", tmp->value, key);
                free(tmp);
            }
            else{
                Pointer = Pointer->next;
            }
        }

        if (counter == 0){
            printf("Key not found. Cannot delete.\n");
        }
        return;
    }
}

// 長度 llen
void llen(NodeForList* head, const char* key)
{
    if (head == NULL)
    {
        printf("Database is Empty. Cannot get Length.\n");
        return;
    }
    else
    {
        NodeForList* Pointer = head;
        int num = 0;
        while (Pointer != NULL)
        {
            if (strcmp(Pointer->key, key) == 0)
            {
                num++;
            }
            Pointer = Pointer->next;
        }

        if(num != 0){
            printf("The length of [%s] is [%d]\n", key, num);
            return;
        }       
    }
    printf("Not Found [%s] in Database. Cannot get Length.\n", key);
}

// 列印範圍 lrange
void lrange(NodeForList* head, NodeForList* bottom, const char* key, int start, int stop)
{
    if (head == NULL)
    {
        printf("Database is Empty. Cannot get Values.\n");
        return;
    }
    else
    {
        NodeForList* Pointer1 = head;
        int num = 0;
        while (Pointer1 != NULL)
        {
            if (strcmp(Pointer1->key, key) == 0)
            {
                num++;
            }
            Pointer1 = Pointer1->next;
        }

        // printf("num: %d\n", num);

        if(num != 0){
            if(stop < 0){
                stop = num + stop; //把stop改成到哪裡結束
            }
            if(start < 0){
                start = num + start; //把start改成到哪裡開始
            }
            // printf("start: %d\n", start);
            // printf("stop: %d\n", stop);

            if(start <= stop){
                int counter = 0;
                NodeForList* Pointer2 = head;
                while (Pointer2 != NULL){
                    if (strcmp(Pointer2->key, key) == 0){
                        if(counter >= start && counter <= stop){
                        printf("%d) %s\n", counter+1, Pointer2->value);
                        }
                        counter++;
                    }
                    Pointer2 = Pointer2->next;
                }
            }
            // 當start < stop
            else{
                int counter = num-1;
                NodeForList* Pointer2 = bottom;
                while (Pointer2 != NULL){
                    if (strcmp(Pointer2->key, key) == 0){
                        if(counter >= stop && counter <= start){
                        printf("%d) %s\n", counter+1, Pointer2->value);
                        }
                        counter--;
                    }
                    
                    Pointer2 = Pointer2->prev;
                }
            }   
            return;
        }
        else{
            printf("Not Found [%s] in Database. Cannot get Values.\n", key);
        }      
    }
}


//以備不時之需
// 讀取 read
void read_node(NodeForList* head, const char* key)
{
    if (head == NULL)
    {
        printf("Database is Empty. Cannot get Value.\n");
        return;
    }
    else
    {
        NodeForList* Pointer = head;
        while (Pointer != NULL)
        {
            if (strcmp(Pointer->key, key) == 0)
            {
                printf("{\n");
                printf("    Key: %s\n", Pointer->key);
                printf("    Value: %s\n", Pointer->value);
                printf("}\n");
                return;
            }
            Pointer = Pointer->next;
        }
    }
    printf("Not Found [%s] in Database.\n", key);
}

// 更新 update
void update_node(NodeForList* head, const char* key, const char* value)
{
    NodeForList* Pointer = head;
    while (Pointer != NULL)
    {
        if (strcmp(Pointer->key, key) == 0)
        {
            strncpy(Pointer->value, value, VALUE_SIZE);
            printf("Update [%s] Successfully.\n", key);
            return;
        }
        Pointer = Pointer->next;
    }
    printf("[%s] Not Found. Cannot update.\n", key);
}

// 刪除 delete
void del_node(NodeForList** head, NodeForList** bottom, const char* key)
{
    if (*head == NULL)
    {
        printf("Database is Empty. Cannot delete Key.\n");
        return;
    }
    else
    {
        NodeForList* pointer = *head;
        while (pointer != NULL)
        {
            if (strcmp(pointer->key, key) == 0)
            {
                if (pointer == *head)
                {
                    *head = pointer->next;
                    if (*head != NULL) {
                        (*head)->prev = NULL;
                    }
                }
                if (pointer == *bottom)
                {
                    *bottom = pointer->prev;
                    if (*bottom != NULL) {
                        (*bottom)->next = NULL;
                    }
                }
                if (pointer->prev != NULL)
                {
                    pointer->prev->next = pointer->next;
                }
                if (pointer->next != NULL)
                {
                    pointer->next->prev = pointer->prev;
                }

                pointer->prev = NULL;
                pointer->next = NULL;
                free(pointer);
                printf("Delete [%s] Successfully.\n", key);
                return;
            }
            pointer = pointer->next;
        }
        printf("Key not found. Cannot delete.\n");
    }
}

void freeList(NodeForList* head) {
    while (head != NULL) {
        NodeForList* Pointer = head;
        head = head->next;
        Pointer->prev = NULL;
        Pointer->next = NULL;
        free(Pointer);
    }
}



// TREE
TreeDatabase* createTreeDatabase(){
    TreeDatabase* db = (TreeDatabase*) malloc(sizeof(TreeDatabase));
    db->tree = NULL;
    db->numOfTree = 0;
    return db;
}

TreeNode* createTreeNode(const char* key) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    strncpy(node->key, key, KEY_SIZE);
    node->value = NULL;
    node->numValues = 0;
    return node;
}

//創建樹根 
void makeATree(TreeDatabase* db, const char* key) {
    TreeNode* tree = createTreeNode(key);
    db->tree = (TreeNode**)realloc(db->tree, (db->numOfTree + 1) * sizeof(TreeNode*));
    db->tree[db->numOfTree] = tree;
    db->numOfTree++;
    printf("Tree of [%s] has made.\n", key);
}

// 加節點_1
void addTreeNode_1(TreeDatabase* db, const char* treeName, const char* value) {
    TreeNode* root = NULL;
    for(int i = 0; i < db->numOfTree; i++){
        if(strcmp(db->tree[i]->key, treeName) == 0){    //找到該樹
            printf("Found the Tree.\n");
            root = db->tree[i];
            break;
        }
    }
    if(root == NULL){
        printf("Cannot find the Tree.\n");
        return;
    }

    TreeNode* parent = root;
    TreeNode* child = createTreeNode(value);
    
    if (parent->value == NULL) {
        // 初始化 value
        parent->value = (TreeNode**)malloc(sizeof(TreeNode*));
        parent->value[0] = child;
    } 
    else {
        parent->value = (TreeNode**)realloc(parent->value, (parent->numValues + 1) * sizeof(TreeNode*));
        parent->value[parent->numValues] = child;
    }

    // 更新值的数量
    parent->numValues++;
    printf("[%s] has add in [%s]\n", value, treeName);
}

// 加節點_(遞迴)
void addTreeNodeRecursive(TreeNode* parent, const char* nodeName, const char* value) {
    if (strcmp(parent->key, nodeName) == 0) {
        // 找到指定的節點，將值添加到這個節點下
        TreeNode* child = createTreeNode(value);

        if (parent->value == NULL) {
            // 初始化 value
            parent->value = (TreeNode**)malloc(sizeof(TreeNode*));
            parent->value[0] = child;
        }
        else {
            parent->value = (TreeNode**)realloc(parent->value, (parent->numValues + 1) * sizeof(TreeNode*));
            parent->value[parent->numValues] = child;
        }

        // 更新值的数量
        parent->numValues++;
        printf("[%s] has add in [%s]\n", value, nodeName);
        return;
    }

    // 在子樹中遞迴搜索
    for (int i = 0; i < parent->numValues; i++) {
        addTreeNodeRecursive(parent->value[i], nodeName, value);
    }
}
// 加節點_2
void addTreeNode_2(TreeDatabase* db, const char* treeName, const char* nodeName, const char* value) {
    TreeNode* root = NULL;
    for (int i = 0; i < db->numOfTree; i++) {
        if (strcmp(db->tree[i]->key, treeName) == 0) {
            printf("Found the Tree.\n");
            root = db->tree[i];
            addTreeNodeRecursive(root, nodeName, value);
            return;
        }
    }

    // 若未找到樹
    printf("Cannot find the Tree.\n");
}

// 列印節點(遞迴)
void printTreeRecursive(TreeNode* node, int numOfSpace) {
    if (node != NULL) {
        printf("%*sKey: (%s)\n", numOfSpace, " ", node->key);
        for (int i = 0; i < node->numValues; i++) {
            // printf("numValues:%d\n", node->numValues);
            printf("%*sValue: (%s)\n", numOfSpace+4, " ", node->value[i]->key);
            printTreeRecursive(node->value[i], numOfSpace+4);  // 遞迴印出子樹
        }
    }
}
// 列印節點
void printTree(TreeDatabase* db, const char* treeName) {
    for (int i = 0; i < db->numOfTree; i++) {
        if (strcmp(db->tree[i]->key, treeName) == 0) {
            TreeNode* node =  db->tree[i];
            printTreeRecursive(node, 0);
        }
    }
}

// 找尋樹節點(遞迴)
TreeNode* findTreeNodeRecursive(TreeNode* root, const char* nodeName) {
    if (root == NULL) {
        return NULL;
    }

    if (strcmp(root->key, nodeName) == 0) { //如果本節點是要找到節點
        return root;
    }

    // 該節點不是要找的節點
    for (int i = 0; i < root->numValues; i++) { //看下一個value
        TreeNode* foundNode = findTreeNodeRecursive(root->value[i], nodeName); //看其子樹(value)
        if (foundNode != NULL) {
            return foundNode;
        }
    }

    return NULL;
}
// 找尋樹節點
void findTreeNode(TreeDatabase* db, const char* treeName,  const char* nodeName) {
    TreeNode* root = NULL;
    TreeNode* node = NULL;
    for (int i = 0; i < db->numOfTree; i++) {
        if(strcmp(db->tree[i]->key, treeName) == 0){    //找到該樹
            printf("Found the Tree.\n");
            root = db->tree[i];
            node = findTreeNodeRecursive(root, nodeName);
            break;
        }
    }   
    if (node != NULL) {
        printf("Values under node [%s]:\n", nodeName);
        for (int j = 0; j < node->numValues; j++) {
            printf("  (%s)\n", node->value[j]->key);
        }
        return;
    }
    
    printf("Node [%s] not found.\n", nodeName);
}

void freeNode(TreeNode* node) {
    if (node == NULL) {
        return;
    }

    // 遞迴釋放子樹的內存
    for (int i = 0; i < node->numValues; i++) {
        freeNode(node->value[i]);
    }

    // 釋放節點自身的內存
    free(node->value);
    free(node);
}

// 刪除樹節點(遞迴)
void deleteNodeRecursive(TreeNode* root, const char* nodeName) {
    if (root == NULL) {
        return;
    }

    // 在value中遞迴搜索
    for (int i = 0; i < root->numValues; i++) {
        if (strcmp(root->value[i]->key, nodeName) == 0) {   // 找到該節點
            // 刪除該節點及所有value
            freeNode(root->value[i]);
            // 將其他value往前搬
            for (int j = i; j < root->numValues - 1; j++) {
                root->value[j] = root->value[j + 1];
            }
            root->numValues--;
            root->value = (TreeNode**)realloc(root->value, root->numValues * sizeof(TreeNode*));
            return;
        } 
        else {// 找不到該節點
            // 在value中繼續遞迴搜索
            deleteNodeRecursive(root->value[i], nodeName);
        }
    }
}
// 刪除樹節點
void deleteTreeNode(TreeDatabase* db, const char* treeName, const char* nodeName) {
    for (int i = 0; i < db->numOfTree; i++) {
        TreeNode* root = db->tree[i];
        if (strcmp(root->key, treeName) == 0) { //找到該樹
            printf("Found the Tree.\n");
            deleteNodeRecursive(root, nodeName);
            printf("Node [%s] and its values have been deleted.\n", nodeName);
            return;
        }
    }

    printf("Cannot find the Tree.\n");
}

void freeTreeDatabase(TreeDatabase* db) {
    if (db == NULL) {
        return;
    }

    // 釋放每個樹的內存
    for (int i = 0; i < db->numOfTree; i++) {
        freeNode(db->tree[i]);
    }

    // 釋放樹陣列的內存
    free(db->tree);

    // 釋放資料庫結構本身的內存
    free(db);
}
// ============================================================

// SET
DatabaseForSet* createDatabaseForSet() {
    DatabaseForSet* db = (DatabaseForSet*)malloc(sizeof(DatabaseForSet));
    db->numSets = 0;
    db->sets = NULL;
    return db;
}

MEMBER *createMember(const int score, const char *member){
    MEMBER *newNode = (MEMBER *)malloc(sizeof(MEMBER));
    if (newNode == NULL){
        perror("Memory allocation failed");
        exit(1);
    }
    newNode->score = score;
    strncpy(newNode->member, member, MEMBER_SIZE);
    newNode->next = NULL;
    return newNode;
}


void addAMember(KeyofSet *set, const char *key, const int score, const char *member) {
    // 找尋一樣的member
    MEMBER *head = set->members;
    MEMBER *current = head;
    MEMBER *prev = head;
    while (current != NULL && (strcmp(current->member, member) != 0)){    //到底部or找到一樣的member跳出
        prev = current;
        current = current->next;
    }

    if(current == NULL){    // 如果找不到member 
        // printf("current=NULL\n");  
        current = head;
        prev = NULL;  // 將 prev 設置為 NULL
        while (current != NULL){
            if(current->score > score){
                MEMBER *newMember = createMember(score, member);
                if (prev == NULL) {
                    // 插入在開頭
                    newMember->next = set->members;
                    set->members = newMember;
                } 
                else {
                    // 插入在中間
                    prev->next = newMember;
                    newMember->next = current;
                }
                break;
            }
            prev = current;
            current = current->next;
        }

        if(current == NULL){    //加在最後
            MEMBER *newMember = createMember(score, member);
            prev->next = newMember;
        }
        printf("[%s] add in [%s] Successfully.\n", member, key);
        return;
    }
    
    //找到對應的member
    if(current->score == score){    //score一樣
        printf("[%s] has been add in [%s] before. Scores are the same.\n", member, key);
        return;
    }
    else{
        //刪除member再重新加入
        if(prev == current){    //如果在頭
            set->members = current->next;
        }
        prev->next = current->next;
        free(current);
        addAMember(set, key, score, member);
        printf("Score of [%s] has been update to [%d].\n", member, score);
    }
}

void ZADD(DatabaseForSet* db, const char *key, const int score, const char *member){
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            addAMember(db->sets[i], key, score, member);
            return;
        }
    }

    // printf("Cannot find key\n");
    // 找不到key 生成一個set
    db->sets = (KeyofSet **)realloc(db->sets, sizeof(KeyofSet *) * (size_t)(db->numSets + 1));
    db->sets[db->numSets] = (KeyofSet *)malloc(sizeof(KeyofSet));
    strcpy(db->sets[db->numSets]->key, key);
    //初始化一個set
    db->sets[db->numSets]->members = createMember(score, member);
    db->numSets++;
    printf("[%s] add in [%s] Successfully.\n", member, key);
    return; 
}

void ZCARD(DatabaseForSet *db, const char *key){
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            int count = 0;
            MEMBER *set_pointer = db->sets[i]->members; //head
            while(set_pointer != NULL){
                count++;
                set_pointer = set_pointer->next;
            }
            printf("[%s] has [%d] member.\n", key, count);
            return;
        }
    }
    //找不到key
    printf("[%s] does not exist.\n", key);
}

int getZCARD(DatabaseForSet *db, const char *key){
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            int count = 0;
            MEMBER *set_pointer = db->sets[i]->members; //head
            while(set_pointer != NULL){
                count++;
                set_pointer = set_pointer->next;
            }
            return count;
        }
    }
    //找不到key
    printf("[%s] does not exist.\n", key);
}

void ZCOUNT(DatabaseForSet* db, const char *key, const int min, const int max){
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            int count = 0;
            MEMBER *pointer = db->sets[i]->members; //head
            while(pointer != NULL){
                if((pointer->score >= min) && (pointer->score <= max)){  //在區間內才做累加
                    count++;
                }
                pointer = pointer->next;
            }
            printf("[%s] has [%d] member between [%d] and [%d].\n", key, count, min, max);
            return;
        }
    }
    //找不到key
    printf("[%s] does not exist.\n", key);
}

void ZINTERSTORE(DatabaseForSet* db, const char *newkey, const char *key1, const char *key2){
    MEMBER *set1_pointer = NULL;
    MEMBER *set2_pointer = NULL;
    MEMBER *set2_head = NULL;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key1) == 0) {   //找到Key1
            set1_pointer = db->sets[i]->members;
        }
        if (strcmp(db->sets[i]->key, key2) == 0) {   //找到Key1
            set2_pointer = set2_head = db->sets[i]->members;
        }
    }
    while(set1_pointer != NULL){
        char *member1 = set1_pointer->member;   //member1為member in key1
        int score1 = set1_pointer->score;
        int new_score;
        while (set2_pointer != NULL){
            char *member2 = set2_pointer->member;
            int score2 = set2_pointer->score;
            if(strcmp(member1, member2) == 0){  //有一樣的member
                new_score = score1 + score2;
                ZADD(db, newkey, new_score, member1);
                break;
            }
            set2_pointer = set2_pointer->next;
        }
        set2_pointer = set2_head;
        set1_pointer = set1_pointer->next;
    }
}

void ZUNIONSTORE(DatabaseForSet* db, const char *newkey, const char *key1, const char *key2){
    MEMBER *set1_pointer = NULL;
    MEMBER *set2_pointer = NULL;
    MEMBER *set1_head = NULL;
    MEMBER *set2_head = NULL;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key1) == 0) {   //找到Key1
            set1_pointer = set1_head = db->sets[i]->members;
        }
        if (strcmp(db->sets[i]->key, key2) == 0) {   //找到Key2
            set2_pointer = set2_head = db->sets[i]->members;
        }
    }
    while(set1_pointer != NULL){
        // 重新設置指標
        set2_pointer = set2_head;
        char *member1 = set1_pointer->member;   //member1為member in key1
        int score1 = set1_pointer->score;
        int new_score;
        while (set2_pointer != NULL){
            char *member2 = set2_pointer->member;   //member2為member in key2
            int score2 = set2_pointer->score;
            if(strcmp(member1, member2) == 0){  //有一樣的member
                new_score = score1 + score2;
                ZADD(db, newkey, new_score, member1);
                break;
            }
            set2_pointer = set2_pointer->next;
        }
        if(set2_pointer == NULL){
            ZADD(db, newkey, score1, member1);  //member1有member2沒有的
        }
        
        set1_pointer = set1_pointer->next;
    }
    // 對第二個set進行迭代，唯有和第一個set不一樣時再加到聯集中
    set1_pointer = set1_head;
    set2_pointer = set2_head;

    while(set2_pointer != NULL){
        char *member2 = set2_pointer->member;   //member2為member in key2
        while (set1_pointer != NULL){
            char *member1 = set1_pointer->member;   //member1為member in key1
            if(strcmp(member1, member2) == 0){
                break;
            }
            set1_pointer = set1_pointer->next;
        }
        if(set1_pointer == NULL){   //找完第一個set發現都沒有
            ZADD(db, newkey, set2_pointer->score, member2);
        }
        // 重新設置指標
        set1_pointer = set1_head;    

        set2_pointer = set2_pointer->next;
    }
}

void ZRANGE(DatabaseForSet* db, const char *key, int start, int stop){
    MEMBER *set_pointer;
    MEMBER *set_head;
    int num = 0;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key1
            set_pointer = set_head = db->sets[i]->members;
            while (set_pointer != NULL){
                // 算長度
                num++;
                set_pointer = set_pointer->next;
            }
        }
    }
    // 重新設置指標
    set_pointer = set_head;
    if(num == 0) {
        printf("[%s] does not exist.\n", key);
        return;
    }
    else{
        if(stop < 0){
            stop = num + stop; //把stop改成到哪裡開始
        }

        int counter = 0;
        while (set_pointer != NULL){
            if(counter >= start && counter <= stop){
                printf("%d) %s:%d\n", counter+1, set_pointer->member, set_pointer->score);
            }
            counter++;
            set_pointer = set_pointer->next;
        }  
    }
}

void ZRANGEBYSCORE(DatabaseForSet* db, const char *key, const int min, const int max){
    MEMBER *set_pointer;
    int count = 0;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            set_pointer = db->sets[i]->members;
            while (set_pointer != NULL){
                if((set_pointer->score >= min) && (set_pointer->score <= max)){
                    printf("%d) %s:%d\n", count+1, set_pointer->member, set_pointer->score);
                    count++;
                }   
                set_pointer = set_pointer->next;
            }
        }
    }
}

void ZRANK(DatabaseForSet* db, const char *key, const char *member){
    MEMBER *set_pointer;
    int count = 0;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            set_pointer = db->sets[i]->members;
            while (set_pointer != NULL){
                if(strcmp(set_pointer->member, member) == 0){
                    printf("The rank of [%s] in [%s] is [%d].\n", member, key, count);
                    return;
                }
                count++;
                set_pointer = set_pointer->next;
            }
        }
    }
}

void ZREM(DatabaseForSet* db, const char *key, const char *member) {
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            MEMBER *set_pointer = db->sets[i]->members;
            MEMBER *prev = NULL;
            
            while (set_pointer != NULL) {
                if (strcmp(set_pointer->member, member) == 0) {
                    if (prev == NULL) {     // 要刪除的節點在頭
                        db->sets[i]->members = set_pointer->next;
                    } 
                    else {
                        prev->next = set_pointer->next;
                    }
                    printf("[%s] has been removed from [%s].\n", member, key);
                    free(set_pointer);
                    // 如果整個 key 列表已經空了將整個 set 刪除
                    if (db->sets[i]->members == NULL) {
                        free(db->sets[i]);
                        // 將 key 從數組中刪除
                        for (int j = i; j < db->numSets - 1; j++) {
                            db->sets[j] = db->sets[j + 1];
                        }
                        db->numSets--;
                    }
                    return;
                }
                
                prev = set_pointer;
                set_pointer = set_pointer->next;
            }

            printf("[%s] not found in [%s].\n", member, key);
            return;
        }
    }
    printf("Cannot find the key\n");
}

void ZREMRANGEBYLEX(DatabaseForSet* db, const char *key, char *min, char *max){
    char min_bound_symbol = min[0];
    char max_bound_symbol = max[0];
    int include_min = 0;
    int include_max = 0;
    char *min_member;
    char *max_member;
    if (strlen(min) > 1){
        min_member = min + 1;
    }
    else{
        printf("Format Error.\n");
        return;
    }
    if (strlen(max) > 1){
        max_member = max + 1;
        printf("%s\n", max_member);
    }
    else{
        printf("Format Error.\n");
        return;
    }

    if(min_bound_symbol == '[') include_min = 1;
    else if (min_bound_symbol == '(') include_min = 0;
    else{
        printf("Format Error.\n");
        return;
    }
    if(max_bound_symbol == '[') include_max = 1;
    else if (max_bound_symbol == '(') include_max = 0;
    else{
        printf("Format Error.\n");
        return;
    }

    int reach_range = 0;
    MEMBER *set_pointer;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            set_pointer = db->sets[i]->members;
            while (set_pointer != NULL){
                char *member = set_pointer->member;
                set_pointer = set_pointer->next;
                // 要刪除min和max
                if(include_min && include_max){
                    // printf("要刪除min和max\n");
                    if(strcmp(member, min_member) == 0){
                        reach_range = 1;
                    }
                    
                    if(reach_range){
                        ZREM(db, key, member);
                    }

                    if(strcmp(member, max_member) == 0){
                        reach_range = 0;
                    }
                }
                // 要刪除min 不要刪掉max
                else if(include_min && !include_max){
                    // printf("要刪除min 不要刪掉max\n");
                    if(strcmp(member, min_member) == 0){
                        reach_range = 1;
                    }

                    if(strcmp(member, max_member) == 0){
                        reach_range = 0;
                    }

                    if(reach_range){
                        ZREM(db, key, member);
                    }
                }
                // 不要刪掉min 刪除max
                else if(!include_min && include_max){
                    // printf("不要刪掉min 刪除max  reach_range:%d\n", reach_range);
                    if(reach_range){
                        ZREM(db, key, member);
                    }

                    if(strcmp(member, min_member) == 0){
                        reach_range = 1;
                    }

                    if(strcmp(member, max_member) == 0){
                        reach_range = 0;
                    }
                }
                //min和max都不要刪除
                else{
                    // printf("min和max都不要刪除\n");
                    if(strcmp(member, max_member) == 0){
                        reach_range = 0;
                    }
                    
                    if(reach_range){
                        ZREM(db, key, member);
                    }

                    if(strcmp(member, min_member) == 0){
                        reach_range = 1;
                    }
                }
            }
        }
    }

}

void ZREMRANGEBYRANK(DatabaseForSet* db, const char *key, const int start, const int stop){
    MEMBER *set_pointer;
    int rank = 0;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            set_pointer = db->sets[i]->members;
            while (set_pointer != NULL){
                char *member = set_pointer->member;
                set_pointer = set_pointer->next;
                if((rank >= start) && (rank <= stop)){
                    ZREM(db, key, member);
                }
                rank++;
            }
        }
    }
}

void ZREMRANGEBYSCORE(DatabaseForSet* db, const char *key, const int min, const int max){
    MEMBER *set_pointer;
    for (int i = 0; i < db->numSets; i++) {
        if (strcmp(db->sets[i]->key, key) == 0) {   //找到Key
            set_pointer = db->sets[i]->members;
            while (set_pointer != NULL){
                char *member = set_pointer->member;
                if((set_pointer->score >= min) && (set_pointer->score <= max)){
                    set_pointer = set_pointer->next;
                    ZREM(db, key, member);
                }
                else{
                    set_pointer = set_pointer->next;
                }
            }
        }
    }
}


// HASH
NodeForHash *createNodeForHash(const char *key, const char *value) {
    NodeForHash *newNode = (NodeForHash *)malloc(sizeof(NodeForHash));
    if (newNode != NULL) {
        strcpy(newNode->key, key);
        strcpy(newNode->value, value);
        newNode->next = NULL;
    }
    return newNode;
}

// 初始化雜湊表
void initHashTable(HashTable *hashTable, const char *field, int table_size) {
    strcpy(hashTable->field, field);
    hashTable->table_size = table_size;
    hashTable->table = (NodeForHash **)malloc(sizeof(NodeForHash *) * table_size);
    for (int i = 0; i < table_size; i++) {
        hashTable->table[i] = NULL;
    }
    hashTable->size = 0;  // 初始化項目數量
    printf("[%s] has set in the database.\n", field);
}

// 初始化雜湊表資料庫
DatabaseForHash* createDatabaseForHash() {
    DatabaseForHash* db = (DatabaseForHash*)malloc(sizeof(DatabaseForHash));
    db->hashTables = NULL;
    db->numHashTables = 0;
    return db;
}

HashTable* searchForField(DatabaseForHash* db, const char *field){
    for (int i = 0; i < db->numHashTables; i++) {
        if (strcmp(db->hashTables[i]->field, field) == 0) {   //找到Field
        // printf("Found [%s].\n", field);
            return db->hashTables[i];
        }
    }
    // 找不到Field
    // printf("Cannot find [%s].\n", field);
    return NULL;
}

// 雜湊函數
int hashFunction(const char *key, const int table_size) {
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash += key[i];
    }
    return hash % table_size;
}

void insertItemForHash(HashTable *hashTable, const char *key, const char *value) {
    int index = hashFunction(key, hashTable->table_size);
    
    NodeForHash *newNode = createNodeForHash(key, value);
    
    // 將新節點插入到槽的開頭(Linked List)
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;

    // 更新項目數量
    hashTable->size++;

    printf("Set [%s: %s] Successfully.\n", key, value);

    // 在插入後檢查Load Factor
    checkLoadFactor(hashTable);
}


// 插入一個項目
void insertItemForHash_withoutLF(HashTable *hashTable, const char *key, const char *value) {
    int index = hashFunction(key, hashTable->table_size);
    
    NodeForHash *newNode = createNodeForHash(key, value);
    
    // 將新節點插入到槽的開頭(Linked List)
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;

    // 更新項目數量
    hashTable->size++;

    printf("Set [%s: %s] Successfully.\n", key, value);

}

// 檢查Load Factor，調整哈希表大小
void checkLoadFactor(HashTable *hashTable) {
    float loadFactor = (float)hashTable->size / hashTable->table_size;
    printf("(Load Factor of [%s]: %f)\n", hashTable->field, loadFactor);

    // 根據需要調整哈希表大小
    if (loadFactor > LOAD_FACTOR_THRESHOLD) {
        printf("Enlarge Hash Table Size.\n");
        int newSize = hashTable->table_size * 2;
        resizeHashTable(hashTable, newSize);  // 放大哈希表
    } else if (loadFactor < MIN_LOAD_FACTOR_THRESHOLD && hashTable->table_size > INITIAL_TABLE_SIZE) {
        printf("Shrink Hash Table Size.\n");
        int newSize = hashTable->table_size / 2;
        resizeHashTable(hashTable, newSize);  // 縮小哈希表，且避免過度縮小
    }
}

// 調整哈希表的大小
void resizeHashTable(HashTable *ht, int newSize) {
    HashTable newHashTable;
    initHashTable(&newHashTable, ht->field, newSize);

    // 將所有現有項目重新插入到新表中
    for (int i = 0; i < ht->table_size; i++) {
        NodeForHash *current = ht->table[i];
        while (current != NULL) {
            insertItemForHash_withoutLF(&newHashTable, current->key, current->value);
            current = current->next;
        }
    }

    // 釋放舊表
    freeHashTable(ht);

    // 更新哈希表的大小
    *ht = newHashTable;

    float loadFactor = (float) ht->size / ht->table_size;
    printf("(Load Factor of [%s]: %f)\n", ht->field, loadFactor);
}

void freeHashTable(HashTable *hashTable) {
    for (int i = 0; i < hashTable->table_size; i++) {
        NodeForHash *current = hashTable->table[i];
        while (current != NULL) {
            NodeForHash *next = current->next;
            free(current);
            current = next;
        }
        hashTable->table[i] = NULL; // 將該槽設為空，避免懸空指標
    }
}

// 查找項目
NodeForHash* searchHashNode(const HashTable *hashTable, const char *key) {
    int index = hashFunction(key, hashTable->table_size);
    
    NodeForHash *pointer = hashTable->table[index];
    
    // 在Linked List中查找
    while (pointer != NULL) {
        if (strcmp(pointer->key, key) == 0) {
            // 找到了
            return pointer;
        }
        pointer = pointer->next;
    }
    
    // 沒有找到
    return NULL;
}

void HSET(DatabaseForHash *db, const char *key, const char *field, const char *value){
    HashTable* ht = searchForField(db, field);

    // 找不到Field
    if(ht == NULL){
        db->hashTables = (HashTable **)realloc(db->hashTables, sizeof(HashTable *) * (size_t)(db->numHashTables + 1));
        db->hashTables[db->numHashTables] = (HashTable *)malloc(sizeof(HashTable));
        // 初始化一個Hash
        initHashTable(db->hashTables[db->numHashTables], field, INITIAL_TABLE_SIZE);
        db->numHashTables++;
        ht = db->hashTables[db->numHashTables-1];

        insertItemForHash(ht, key, value);
        return;
    }

    NodeForHash *pointer = searchHashNode(ht, key);
    if(pointer == NULL){
        // 新的key
        insertItemForHash(ht, key, value);
    }
    else{
        // 更新value
        strcpy(pointer->value, value);
        printf("Update [%s] Successfully.\n", key);
    }
    
}

void HGET(DatabaseForHash *db, const char *key, const char *field){
    HashTable* ht = searchForField(db, field);

    if(ht == NULL){
        printf("Not Found [%s] in Database.\n", field);
        return;
    }
    else{
        NodeForHash *pointer = searchHashNode(ht, key);
        if (pointer == NULL){
            printf("Not Found [%s] in [%s] hashtable.\n", key, field);
            return;
        }
        else {
            printf("Key: [%s]  Value: [%s]\n", key, pointer->value);
            return;
        }
        
    }
}

void HDEL(DatabaseForHash *db, const char *key, const char *field, const int isEXPIRE) {
    HashTable* ht = searchForField(db, field);

    if (ht == NULL) {
        printf("Not Found [%s] in [%s] hashtable.\n", key, field);
        return;
    }

    int index = hashFunction(key, ht->table_size);
    NodeForHash *current = ht->table[index];
    NodeForHash *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // 找到要刪除的節點
            if (prev == NULL) {
                // 節點在Linked List的開頭
                ht->table[index] = current->next;
            } 
            else {
                // 節點在Linked List中間或尾巴
                prev->next = current->next;
            }

            // 釋放節點
            free(current);

            // 更新項目數量
            ht->size--;

            if(isEXPIRE == 1){
                printf("\n");
                printf("Deleted [%s] Successfully.\n", key);
            }
            else{
                printf("Deleted [%s] Successfully.\n", key);
            }

            // 在刪除後檢查Load Factor
            checkLoadFactor(ht);
            // printf("%d", ht->table_size);
            return;
        }

        prev = current;
        current = current->next;
    }

    // 找不到要刪除的節點
    printf("Key [%s] not found in [%s].\n", key, field);
}

void freeHashDatabase(DatabaseForHash *db) {
    // 釋放每一個哈希表的記憶體
    for (int i = 0; i < db->numHashTables; i++) {
        freeHashTable(db->hashTables[i]);
        free(db->hashTables[i]);
    }

    // 釋放哈希表的陣列
    free(db->hashTables);

    // 釋放資料庫本身
    free(db);
}

// void EXPIRE(DatabaseForHash* db, const char* key, const int time){
//     clock_t start_time = clock();
//     clock_t duration = time * CLOCKS_PER_SEC;
//     clock_t elapsed_time = clock() - start_time;
//     printf("Waiting:");
//     while (elapsed_time < duration) {
//         if(elapsed_time % CLOCKS_PER_SEC == 0){
//             printf(".");
//         }
//         elapsed_time = clock() - start_time;
//     }
//     printf("\n");

//     for (int i = 0; i < db->numHashTables; i++) {
//         char *field = db->hashTables[i]->field;
//         HDEL(db, key, field, 1);
//     }
// }

void EXPIRE(DatabaseForHash* db, const char* key, const int time){
    HANDLE threadHandle;
    DWORD threadId;
    EXPIRE_input *input = (EXPIRE_input*)malloc(sizeof(EXPIRE_input));
    if (input == NULL) {
        fprintf(stderr, "Error allocating memory for input\n");
        return;
    }

    input->db = db;
    strcpy(input->key, key);
    input->time = time;
    printf("[%s] will be expired in [%d]s.\n", key, time);
    threadHandle = CreateThread(NULL, 0, countDownToDEL, input, 0, NULL);   //創建Thread
    if (threadHandle == NULL) {
        printf("Error creating thread\n");
        free(input);
        return;
    }
}

DWORD WINAPI countDownToDEL(LPVOID lpParam) {
    EXPIRE_input* input = (EXPIRE_input*)lpParam;
    Sleep(input->time * 1000);    //線程休眠一段時間(此函式的單位為毫秒所以要乘以1000)
    // printf("%d", input->time);
    for (int i = 0; i < input->db->numHashTables; i++) {    //將所有有包含該key名稱的field都刪除
        char *field = input->db->hashTables[i]->field;
        HDEL(input->db, input->key, field, 1);
    }
    
    printf("> ");
    free(input);
    return 0;
}

