
typedef struct node0 {
    int value;
    struct node0 * next;
} Node;
void add(int a);
void addFirst(int a);
void addLast(int a);
void delete(int a);
bool isEmpty();