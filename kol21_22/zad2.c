#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int len;
    double *results;

} Data;

typedef struct tagQueueElement
{
    Data data;
    struct tagQueueElement *next;
} QueueElement;

typedef QueueElement Queue;

void init_queue(Queue *pqueue)
{
    pqueue->next = NULL;
    
}

void dump_tab(Data pdata)
{
    for (int i = 0; i < pdata.len; i++)
    {
        printf("%lf ", (pdata.results)[i]);
    }
    printf("\n");
}

void dump_queue(Queue *h)
{

    Queue *head = h;
    head = head->next;
    //printf("%d=zu",head);
    while (head != NULL)
    {
        dump_tab(head->data);
        head = head->next;
    }
    printf("\n");
}
void free_queue(Queue *pqueue)
{
    Queue *ptr;
    Queue *tmp;
    ptr = pqueue->next;

    while (ptr->next != NULL)
    {
        tmp = ptr->next;
        //printf("%zu\n",ptr);
        free(ptr);
        ptr=tmp;
    }
    pqueue->next=NULL;
}

void push_copy(Queue *pqueue, const Data *pdata)
{
    QueueElement *new = malloc(sizeof(QueueElement));
    new->data.len = pdata->len;
    new->data.results = malloc((pdata->len) * sizeof(double));
    for (int i = 0; i < (pdata->len); i++)
    {
        ((new->data).results)[i] = (pdata->results)[i];
    }
    new->next = NULL;
    if (pqueue->next == NULL)
    {
        printf("NULL\n");
        pqueue ->next= new;
        //dump_queue(pqueue);
        // return;
    }
    else
    {
        pqueue = pqueue->next;
        Queue *prev = pqueue;
        pqueue = pqueue->next;
        while (pqueue != NULL)
        {
            prev = pqueue;
            pqueue = pqueue->next;
        }
        prev->next = new;
    }
}
int get_total_count(const Queue *pqueue)
{
    Queue *ptr = pqueue->next;
    int count = 0;
    while (ptr != NULL)
    {
        count += ptr->data.len;
        ptr = ptr->next;
    }
    return count;
}
int peek(const Queue *pqueue, Data *pdata)
{

    if (pqueue->next == NULL)
    {
        return 0;
    }
    pqueue = pqueue->next;
    pdata->len = pqueue->data.len;
    for (int i = 0; i < pdata->len; i++)
    {
        pdata->results[i] = pqueue->data.results[i];
    }

    return 1;
}

int pop(Queue *pqueue, Data *pdata)
{

    if (pqueue->next == NULL)
    {
        return -1;
    }
    peek(pqueue, pdata);
    Queue *ptr=pqueue->next;
    pqueue ->next= ptr->next;
    free(ptr);

    return 1;
}

int main()
{
    QueueElement Q;
    //Q.next=NULL;
    init_queue(&Q);
    dump_queue(&Q);
    // printf("%zu", Q);
    Data *pdata = malloc(sizeof(Data));
    int len;
    scanf("%d", &len);
    pdata->len = len;
    pdata->results = malloc(sizeof(double) * pdata->len);
    for (int i = 0; i < pdata->len; i++)
    {
        scanf("%lf", &((pdata->results)[i]));
    }

    // dump_tab(*pdata);
    push_copy(&Q, pdata);
    // for (int i = 0; i < Q->data.len; i++)
    // {
    //     printf("%lf ", ((Q->data).results)[i]);
    // }
    // printf("\n");
    dump_queue(&Q);

    scanf("%d", &len);
    pdata->len = len;
    pdata->results = malloc(sizeof(double) * pdata->len);
    for (int i = 0; i < pdata->len; i++)
    {
        scanf("%lf", &((pdata->results)[i]));
    }
    //dump_tab(*pdata);
    push_copy(&Q, pdata);
    dump_queue(&Q);
    printf("%d\n",get_total_count(&Q));
    pop(&Q,pdata);
    dump_tab(*pdata);
    //printf("%zu a",&Q);
    printf("\n");
    dump_queue(&Q);
    free_queue(&Q);
    
}