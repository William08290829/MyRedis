#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include "mylib.h"

int main()
{
    // KEY-VALUE
    Database* db = createDatabase();
    char input[INPUT_SIZE];

    // LIST
    NodeForList* head = NULL;
    NodeForList* bottom = NULL; 
    char input_for_list[INPUT_SIZE];

    // TREE
    TreeDatabase* db_for_Tree = createTreeDatabase();


    //SET
    DatabaseForSet *db_for_Set = createDatabaseForSet();

    // HASH
    DatabaseForHash *db_for_hash = createDatabaseForHash();

    printf("KEY:(get/set/update/del)\n");
    printf("LIST:(lpush/rpush/lpop/rpop/llen/lrange)\n");
    printf("SET:(zadd/zcard/zcount/zinterstore/zunionstore/zrange/zrangebyscore/zrank/zrem/zremrangebylex/zremrangebyrank/zremrangebyscore)\n");
    printf("HASH:(hset/hget/hdel/expire)\n");
    printf("EXIT:0\n");
    printf("Enter a command: \n");

    do
    {
        printf("> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        strcpy(input_for_list, input);
        
        char* command_ptr;
        char* key_ptr;

        // pointer
        command_ptr = strtok(input_for_list, " ");
        key_ptr = strtok(NULL, " ");


        if (command_ptr != NULL && key_ptr != NULL)
        {
            // KEY-VALUE
            // SET
            if (strcmp(command_ptr, "set") == 0)
            {
                char *value_ptr = strtok(NULL, " ");
                if(value_ptr != NULL){
                    insert(db, key_ptr, value_ptr);
                }
                else{
                    printf("Invalid command\n"); 
                }
            }

            // GET
            else if (strcmp(command_ptr, "get") == 0)
            {
                srand(time(NULL));
                double GET_START = 0, GET_END = 0;
                GET_START = clock();
                read(db, key_ptr);
                GET_END = clock();

                double get_time = (GET_END - GET_START) / CLOCKS_PER_SEC;
                printf("Took %lf sec to get [%s]\n", get_time, key_ptr);
            }

            // UPDATE
            else if (strcmp(command_ptr, "update") == 0)
            {
                char *value_ptr = strtok(NULL, " ");
                if(value_ptr != NULL){
                    update(db, key_ptr, value_ptr);
                }
                else{
                    printf("Invalid command\n"); 
                }
            }
            
            // DEL
            else if (strcmp(command_ptr, "del") == 0)
            {
                del(db, key_ptr);
            }
            
            
            // LIST
            // LPUSH
            else if (strcmp(command_ptr, "lpush") == 0)
            {
                char *value_ptr = strtok(NULL, " ");
                while (value_ptr != NULL)
                {
                    insertLeft_node(&head, &bottom, key_ptr, value_ptr);
                    value_ptr = strtok(NULL, " ");
                }
            }
            // RPUSH
            else if (strcmp(command_ptr, "rpush") == 0)
            {
                char *value_ptr = strtok(NULL, " ");
                while (value_ptr != NULL)
                {
                    insertRight_node(&head, &bottom, key_ptr, value_ptr);
                    value_ptr = strtok(NULL, " ");
                }
            }
            
            // LPOP
            else if (strcmp(command_ptr, "lpop") == 0)
            {
                int num = atoi(strtok(NULL, " "));
                lpop(&head, &bottom, key_ptr, num);
            }
            
            // RPOP
            else if (strcmp(command_ptr, "rpop") == 0)
            {
                int num = atoi(strtok(NULL, " "));
                rpop(&head, &bottom, key_ptr, num);
            }

            // LLEN
            else if (strcmp(command_ptr, "llen") == 0)
            {
                llen(head, key_ptr);
            }

            // LRANGE
            else if (strcmp(command_ptr, "lrange") == 0)
            {
                int start = atoi(strtok(NULL, " "));
                int stop = atoi(strtok(NULL, " "));
                lrange(head, bottom, key_ptr, start, stop);
            }

            // TREE
            else if (strcmp(command_ptr, "maketree") == 0)
            {
                makeATree(db_for_Tree, key_ptr);
            }

            else if (strcmp(command_ptr, "addtreenode1") == 0)
            {
                char *treeName = key_ptr;
                key_ptr = strtok(NULL, " ");
                addTreeNode_1(db_for_Tree, treeName, key_ptr);
            }

            else if (strcmp(command_ptr, "addtreenode2") == 0)
            {
                char *treeName = key_ptr;
                char *nodeName = strtok(NULL, " ");
                char *value_ptr = strtok(NULL, " "); 
                addTreeNode_2(db_for_Tree, treeName, nodeName, value_ptr);
            }

            else if (strcmp(command_ptr, "printtree") == 0)
            {
                printTree(db_for_Tree, key_ptr);
            }

            else if (strcmp(command_ptr, "findtreenode") == 0)
            {
                char *treeName = key_ptr;
                char *nodeName = strtok(NULL, " ");
                findTreeNode(db_for_Tree, treeName, nodeName);
            }

            else if (strcmp(command_ptr, "deltreenode") == 0)
            {
                char *treeName = key_ptr;
                char *nodeName = strtok(NULL, " ");
                deleteTreeNode(db_for_Tree, treeName, nodeName);
            }


            //SET
            //ZADD
            else if (strcmp(command_ptr, "zadd") == 0) {
                char *score_ptr = strtok(NULL, " ");
                while (score_ptr != NULL) {
                    int score = atoi(score_ptr);
                    char *member_ptr = strtok(NULL, " ");
                    ZADD(db_for_Set, key_ptr, score, member_ptr);

                    // 
                    score_ptr = strtok(NULL, " ");
                }
            }

            //ZCARD
            else if (strcmp(command_ptr, "zcard") == 0) {
                ZCARD(db_for_Set, key_ptr);
            }
            
            //ZCOUNT
            else if (strcmp(command_ptr, "zcount") == 0) {
                char *min_ptr = strtok(NULL, " ");
                char *max_ptr = strtok(NULL, " ");
                int min = atoi(min_ptr);
                int max = atoi(max_ptr);
                ZCOUNT(db_for_Set, key_ptr, min, max);
            }

            //ZINTERSTORE
            else if (strcmp(command_ptr, "zinterstore") == 0) {
                char *key1 = strtok(NULL, " ");
                char *key2 = strtok(NULL, " ");
                ZINTERSTORE(db_for_Set, key_ptr, key1, key2);
            }

            //ZUNIONSTORE
            else if (strcmp(command_ptr, "zunionstore") == 0) {
                char *key1 = strtok(NULL, " ");
                char *key2 = strtok(NULL, " ");
                ZUNIONSTORE(db_for_Set, key_ptr, key1, key2);
            }

            //ZRANGE
            else if (strcmp(command_ptr, "zrange") == 0) {
                int start = atoi(strtok(NULL, " "));
                int stop = atoi(strtok(NULL, " "));
                ZRANGE(db_for_Set, key_ptr, start, stop);
            }

            //ZRANGEBYSCORE
            else if (strcmp(command_ptr, "zrangebyscore") == 0) {
                int min = atoi(strtok(NULL, " "));
                int max = atoi(strtok(NULL, " "));
                ZRANGEBYSCORE(db_for_Set, key_ptr, min, max);
            }

            //ZRANK
            else if (strcmp(command_ptr, "zrank") == 0) {
                char *member = strtok(NULL, " ");
                ZRANK(db_for_Set, key_ptr, member);
            }

            //ZREM
            else if (strcmp(command_ptr, "zrem") == 0) {
                char *member = strtok(NULL, " ");
                ZREM(db_for_Set, key_ptr, member);
            }

            //ZREMRANGEBYLEX
            else if (strcmp(command_ptr, "zremrangebylex") == 0) {
                char *min = strtok(NULL, " ");
                char *max = strtok(NULL, " ");
                ZREMRANGEBYLEX(db_for_Set, key_ptr, min, max);
            }

            //ZREMRANGEBYRANK
            else if (strcmp(command_ptr, "zremrangebyrank") == 0) {
                int start = atoi(strtok(NULL, " "));
                int stop = atoi(strtok(NULL, " "));
                ZREMRANGEBYRANK(db_for_Set, key_ptr, start, stop);
            }

            //ZREMRANGEBYSCORE
            else if (strcmp(command_ptr, "zremrangebyscore") == 0) {
                int min = atoi(strtok(NULL, " "));
                int max = atoi(strtok(NULL, " "));
                ZREMRANGEBYSCORE(db_for_Set, key_ptr, min, max);
            }


            // hset 1 field1 Value1
            // hset 2 field1 Value2
            // hset 3 field1 Value3
            // hset 4 field1 Value4
            // hset 5 field1 Value5
            // hset 6 field1 Value6
            // hset 7 field1 Value7
            // hset 8 field1 Value8
            // hset 9 field1 Value9
            // hset 10 field1 Value10
            // hset 11 field1 Value11
            // hdel 1 field1
            // hdel 2 field1
            // hdel 3 field1
            // hdel 4 field1
            // hdel 5 field1
            // hdel 6 field1
            // hdel 7 field1
            // hdel 8 field1

            // HASH
            // HSET
            else if (strcmp(command_ptr, "hset") == 0) {
                char *field_ptr = strtok(NULL, " ");
                char *value_ptr = strtok(NULL, " ");
                HSET(db_for_hash, key_ptr, field_ptr, value_ptr);
            }
            // HGET
            else if (strcmp(command_ptr, "hget") == 0) {
                char *field_ptr = strtok(NULL, " ");
                HGET(db_for_hash, key_ptr, field_ptr);
            }
            // HDEL
            else if (strcmp(command_ptr, "hdel") == 0) {
                char *field_ptr = strtok(NULL, " ");
                HDEL(db_for_hash, key_ptr, field_ptr, 0);
            }

            // EXPIRE
            else if (strcmp(command_ptr, "expire") == 0) {
                int time = atoi(strtok(NULL, " "));
                EXPIRE(db_for_hash, key_ptr, time);
            }


            else
            {
               printf("Invalid command\n"); 
            }
            
            
        }

        else if (strcmp(input, "0") == 0)
        {
            printf("END");
            break;
        }

        else
        {
            printf("Invalid command\n");
        }

    } while (1);

    free(db->data);
    free(db);
    freeList(head);
    freeHashDatabase(db_for_hash);

    return 0;
}