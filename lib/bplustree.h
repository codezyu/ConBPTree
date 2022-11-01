
#ifndef _BPLUS_TREE_H
#define _BPLUS_TREE_H
//order是中间节点中叶子节点的最大数目
//entrires是叶子节点中kv对的最大数目
#define BPLUS_MIN_ORDER     3
#define BPLUS_MAX_ORDER     64
#define BPLUS_MAX_ENTRIES   64
#define BPLUS_MAX_LEVEL     10
#include <stdint.h>
#include <pthread.h>
typedef uint64_t key_t;
typedef char* val_t;

struct kvPair{
    key_t key;
    val_t val;
    struct kvPair* next;
};
struct list_head {
        struct list_head *prev, *next;
};

static inline void list_init(struct list_head *link)
{
        link->prev = link;
        link->next = link;
}
//将link插入其中
static inline void
__list_add(struct list_head *link, struct list_head *prev, struct list_head *next)
{
        link->next = next;
        link->prev = prev;
        next->prev = link;
        prev->next = link;
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
        prev->next = next;
        next->prev = prev;
}

// prev next to  prev link next
static inline void list_add(struct list_head *link, struct list_head *prev)
{
        __list_add(link, prev, prev->next);
}

static inline void list_add_tail(struct list_head *link, struct list_head *head)
{
	__list_add(link, head->prev, head);
}

static inline void list_del(struct list_head *link)
{
        __list_del(link->prev, link->next);
        list_init(link);
}

static inline int list_is_first(struct list_head *link, struct list_head *head)
{
	return link->prev == head;
}

static inline int list_is_last(struct list_head *link, struct list_head *head)
{
	return link->next == head;
}

#define list_entry(ptr, type, member) \
        ((type *)((char *)(ptr) - (size_t)(&((type *)0)->member)))

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, typeof(*(pos)), member)

#define list_for_each_safe(pos, n, head) \
        for (pos = (head)->next, n = pos->next; pos != (head); \
                pos = n, n = pos->next)
/**
 * b plus tree basic node
 * 
 *  
 */ 
struct bplus_node {
        //读写锁
        pthread_rwlock_t lock;
        //用于说明是否为叶子节点
        int type;
        int parent_key_idx;
        //指向父节点
        struct bplus_non_leaf *parent;
/** pointer to first node(head) in leaf linked list */
        struct list_head link;
/** */
        int count;
};
//内部节点
struct bplus_non_leaf {
        pthread_rwlock_t lock;
        int type;
        int parent_key_idx;
        struct bplus_non_leaf *parent;
        struct list_head link;
        int children;
/**  key array */
        key_t key[BPLUS_MAX_ORDER - 1];
/** pointers to child node */
        struct bplus_node *sub_ptr[BPLUS_MAX_ORDER];
};
struct bplus_leaf {
        pthread_rwlock_t lock;
        //类型
        int type;
        int parent_key_idx;
        struct bplus_non_leaf *parent;
        struct list_head link;
        int entries;
        //存储的key value数据
        key_t key[BPLUS_MAX_ENTRIES];
        val_t data[BPLUS_MAX_ENTRIES];

};
/** b plus tree structure */
struct bplus_tree {
/**  The actual number of children for a node, referred to here as order */
        int order;
/** number of actual key-value pairs in tree */
        int entries;
/** height of the tree */
        int level;
        struct bplus_node *root;
/** double linked list to search leaf node*/
        struct list_head list[BPLUS_MAX_LEVEL];
};


void bplus_tree_dump(struct bplus_tree *tree);
val_t bplus_tree_get(struct bplus_tree *tree, key_t key);
//插入
int bplus_tree_insert(struct bplus_tree *tree, key_t key, val_t data);
//更新
int bplus_tree_update(struct bplus_tree *tree, key_t key, val_t data);
//删除
int bplus_tree_delete(struct bplus_tree *tree, key_t key);

struct kvPair* bplus_tree_get_range(struct bplus_tree *tree, key_t key1, key_t key2);

struct bplus_tree *bplus_tree_init(int order, int entries);

void bplus_tree_deinit(struct bplus_tree *tree);

#endif  /* _BPLUS_TREE_H */



