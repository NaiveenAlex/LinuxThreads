//https://github.com/csepracticals/DevelopLibrary/tree/master/glthreads

#include <iostream>
#include <string>
#include <cstring>

//traditional
typedef struct dll_node_
{
    void *data;
    struct dll_node_ *pleft;
    struct dll_node_ *pright;

}dll_node_t;


typedef struct  dll_list_
{
    /* data */
    dll_node_t *phead;
    int (*key_match) (void*, void*);
    int (*comp_func) (void*, void*);
}dll_t;

//glue based
typedef struct gll_node_
{
    struct gll_node_ *pleft;
    struct gll_node_ *pright;

}gll_node_t;

typedef struct  gll_list_
{
    /* data */
    gll_node_t *phead;
    int offset;                         //Offset of glnode within the data .
}gll_t;

typedef struct emp_
{
    char name[30];
    unsigned int salary;
    char designation[30];
    unsigned int emp_id;
    gll_node_t glnode;
}emp_t;


//C macro computes the offset of a given field in a given C structure.
#define offset_of(struct_name, struct_field)  (unsigned int)&((struct_name*)0)->struct_field

#define ITERATE_GLL_BEGIN (lst_ptr, node_type, node_ptr)  \
{                                                               \
    gll_node_t* _node = nullptr, *_next = nullptr;              \
    for(_node = lst_ptr->phead; _node; _node = _next)           \
    {                                                           \
        _next = _node->pright;                                  \
        ptr = (struct_type*)((char*)_node - lst_ptr->offset);

#define ITERATE_GLL_END }}

#define gll_node_init(gll_node_t)  \
    gll_node_t->pleft = nullptr;        \
    gll_node_t->pright = nullptr;

static void
_gll_add_next(gll_node_t *curr_node, 
                   gll_node_t *new_node)
{
    if(!curr_node->pright)
    {
        curr_node->pright =  new_node;
        new_node->pleft   = curr_node;
        return ;
    }

    gll_node_t *temp_node = curr_node->pright;
    curr_node->pright= new_node;
    new_node->pleft  = curr_node;
    new_node->pright = temp_node;
    temp_node->pleft = new_node;
}

static void
_gll_remove(gll_node_t *curr_node)
{
    if(!curr_node->pleft)
    {
        if(curr_node->pright)
        {
            curr_node->pright->pleft = nullptr;
            curr_node->pright = nullptr;
            return;
        }
        return;
    }

    if(!curr_node->pright)
    {
        curr_node->pleft->pright = nullptr;
        curr_node->pleft = nullptr;
    }

    curr_node->pleft->pright = curr_node->pright;
    curr_node->pright->pleft = curr_node->pleft;

    curr_node->pleft = nullptr;
    curr_node->pright= nullptr;
}


void 
init_gll(gll_t* gll, unsigned int offset)
{
    gll->phead = nullptr;
    gll->offset= offset;
}

//Add a node to the head
void
gll_add_node(gll_t* gll, gll_node_t* glnode)
{
    glnode->pleft = nullptr;
    glnode->pright= nullptr;

    if(!gll->phead)
    {
        gll->phead = glnode;
        return;
    }

    gll_node_t* head = gll->phead;
    _gll_add_next(glnode, head);
    gll->phead = glnode;
}

//Delete the node from list
void
gll_del_node(gll_t* gll, gll_node_t* glnode)
{
    if(glnode == gll->phead)
    {
        gll->phead = gll->phead->pright;
    }

    _gll_remove(glnode);
}






void 
print_emp_db(emp_t *emp)
{
    printf("Employee name = %s\n", emp->name);
    printf("salary = %u\n", emp->salary);
    printf("designation = %s\n", emp->designation);
    printf("emp_id = %u\n", emp->emp_id);
}

int main()
{

    emp_t *emp1  = (emp_t*)calloc(1, sizeof(emp_t));
    strncpy(emp1->name, "Neha", strlen("Neha"));
    emp1->salary = 50000;
    strncpy(emp1->designation, "HR", strlen("HR"));
    emp1->emp_id = 21;
    gll_node_init((&emp1->glnode));

    emp_t *emp2  = (emp_t*)calloc(1, sizeof(emp_t));
    strncpy(emp2->name, "Abhishek", strlen("Abhishek"));
    emp1->salary = 150000;
    strncpy(emp2->designation, "SE3", strlen("SE3"));
    emp2->emp_id = 32;
    gll_node_init((&emp2->glnode));

    emp_t *emp3  = (emp_t*)calloc(1, sizeof(emp_t));
    strncpy(emp3->name, "Arun", strlen("Arun"));
    emp3->salary = 60000;
    strncpy(emp3->designation, "SE4", strlen("SE4"));
    emp3->emp_id = 27;
    gll_node_init((&emp3->glnode));



    /*create a glthread*/
    gll_t *emp_list = (gll_t*)calloc(1, sizeof(gll_t));
    init_gll(emp_list, offset_of(emp_t, glnode));

    /*insert the records in glthread*/
    gll_add_node(emp_list, &emp1->glnode);
    gll_add_node(emp_list, &emp2->glnode);
    gll_add_node(emp_list, &emp3->glnode);

    /*Walk over glthread*/
    return 0;
}
