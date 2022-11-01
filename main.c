#include <stdio.h>
#include "lib/bplustree.h"
#include <unistd.h>
typedef struct{
    struct bplus_tree * root;
    int num;
}args;
void test(void *arg){
    struct bplus_tree* root=((args*)arg)->root;
    struct kvPair* list= bplus_tree_get_range(root,1,3);
    printf("%d\n",((args*)arg)->num);
    while(list!=NULL){
        printf("%d :%s\n",list->key,list->val);
        list=list->next;
    }
}
void test1( struct bplus_tree * root){
    bplus_tree_update(root,2,"wer2");
}
int main() {
    struct bplus_tree *root=bplus_tree_init(4,4);
//    //插入测试
//    bplus_tree_insert(root,1,"wer");
//    bplus_tree_insert(root,2,"wer1");
//    bplus_tree_insert(root,3,"wer3");
//    printf("%s\n",bplus_tree_get(root,1));
//    //修改测试
//    bplus_tree_update(root,1,"3423");
//    printf("%s\n",bplus_tree_get(root,1));
//    //再次插入
//    bplus_tree_insert(root,1,"wer");
//    printf("%s\n",bplus_tree_get(root,1));
//    //删除测试
//    bplus_tree_delete(root,1);
//    printf("%s\n",bplus_tree_get(root,1));
//    bplus_tree_insert(root,1,"wer");
//    bplus_tree_insert(root,2,"3423");
//    bplus_tree_insert(root,3,"3423");
//    struct kvPair* list= bplus_tree_get_range(root,1,3);
//    while(list!=NULL){
//        printf("%d :%s\n",list->key,list->val);
//        list=list->next;
//    }
    pthread_t readThread[3];
    pthread_t writeThread;

    for (int i = 0; i < 3; ++i)
    {
        args arg={root,i};
        pthread_create(&readThread[i], NULL, test,&arg);
    }
    pthread_join(readThread[0], NULL);
    pthread_create(&writeThread, NULL, test1, root);
    pthread_join(writeThread,NULL);
    pthread_join(readThread[2], NULL);
    pthread_join(readThread[1], NULL);
    return 0;
}
