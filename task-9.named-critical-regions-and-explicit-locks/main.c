#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MAX_LISTS 100

/** 
 * @brief Struct for a linked list node.
 * 
 * This struct represents a node in a singly linked list. Each node contains
 * an integer value and a pointer to the next node in the list.
 */
typedef struct Node {
    int value;          /**< The value stored in the node */
    struct Node* next;  /**< Pointer to the next node in the list */
} Node;

/** 
 * @brief Inserts a new node with a given value at the beginning of the list.
 * 
 * @param head A pointer to the head of the linked list.
 * @param value The integer value to be inserted into the list.
 */
void insert(Node** head, int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = *head;
    *head = new_node;
}

/** 
 * @brief Prints the contents of the linked list.
 * 
 * @param head The head of the linked list.
 * @param label A label for the list (e.g., "List 1").
 * @param method The method used for the list creation (e.g., "Named" or "Generalized").
 */
void print_list(Node* head, const char* label, const char* method) {
    printf("[Method %s] %s: ", method, label);
    for (Node* curr = head; curr != NULL; curr = curr->next)
        printf("%d -> ", curr->value);
    printf("NULL\n");
}

/** 
 * @brief Named mode with two lists using OpenMP tasks and named critical sections.
 *
 * This mode demonstrates the use of OpenMP tasks and named critical sections
 * to manage two separate linked lists.
 */
void named_mode() {
    printf("==== NAMED MODE (two lists, OpenMP tasks) ====\n");

    Node* list1 = NULL;
    Node* list2 = NULL;
    int N = 10;

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {
                #pragma omp task
                {
                    unsigned int seed = (unsigned int)time(NULL) ^ omp_get_thread_num() ^ i;
                    int value = rand_r(&seed) % 100;
                    int choice = rand_r(&seed) % 2;

                    if (choice == 0) {
                        #pragma omp critical(list1_section)
                        insert(&list1, value);
                    } else {
                        #pragma omp critical(list2_section)
                        insert(&list2, value);
                    }
                }
            }
        }
    }

    print_list(list1, "List 1", "Named");
    print_list(list2, "List 2", "Named");
}

/** 
 * @brief Generalized mode with multiple lists using OpenMP tasks and locks.
 *
 * This mode demonstrates the use of OpenMP tasks and locks to handle M lists.
 * The user can specify the number of lists and insertions.
 */
void generalized_mode() {
    printf("\n==== GENERALIZED MODE (M lists, OpenMP tasks + locks) ====\n");

    int M, N;
    printf("Enter the number of lists (M, up to %d): ", MAX_LISTS);
    scanf("%d", &M);
    if (M <= 0 || M > MAX_LISTS) {
        printf("Invalid number of lists.\n");
        return;
    }

    printf("Enter the number of insertions: ");
    scanf("%d", &N);

    Node* lists[MAX_LISTS] = {NULL};
    omp_lock_t locks[MAX_LISTS];

    for (int i = 0; i < M; i++) {
        omp_init_lock(&locks[i]);
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < N; i++) {
                #pragma omp task
                {
                    unsigned int seed = (unsigned int)time(NULL) ^ omp_get_thread_num() ^ i;
                    int value = rand_r(&seed) % 1000;
                    int idx = rand_r(&seed) % M;

                    omp_set_lock(&locks[idx]);
                    insert(&lists[idx], value);
                    omp_unset_lock(&locks[idx]);
                }
            }
        }
    }

    for (int i = 0; i < M; i++) {
        char label[50];
        snprintf(label, sizeof(label), "List %d", i);
        print_list(lists[i], label, "Generalized");
        omp_destroy_lock(&locks[i]);
    }
}

/** 
 * @brief Main function to execute both modes.
 * 
 * This function calls both the named mode and the generalized mode to 
 * demonstrate the two types of list insertions and printing.
 */
int main() {
    named_mode();
    generalized_mode();
    return 0;
}