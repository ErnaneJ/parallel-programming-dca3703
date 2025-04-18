/**
 * @file file_processor.c
 * @brief Parallel file processing using OpenMP tasks
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

/**
 * @brief Node structure for a linked list of filenames
 */
typedef struct Node
{
  char filename[100]; ///< Name of the file
  struct Node *next;  ///< Pointer to the next node
} Node;

/**
 * @brief Creates a new node with the given filename
 * @param name The filename to store in the node
 * @return Pointer to the newly created node
 */
Node *createNode(const char *name)
{
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (newNode == NULL)
  {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  strncpy(newNode->filename, name, sizeof(newNode->filename) - 1);
  newNode->filename[sizeof(newNode->filename) - 1] = '\0';
  newNode->next = NULL;
  return newNode;
}

/**
 * @brief Appends a new node to the end of the linked list
 * @param head Pointer to the head of the linked list
 * @param name Filename to add to the list
 */
void appendNode(Node **head, const char *name)
{
  Node *newNode = createNode(name);
  if (*head == NULL)
  {
    *head = newNode;
  }
  else
  {
    Node *temp = *head;
    while (temp->next != NULL)
      temp = temp->next;
    temp->next = newNode;
  }
}

/**
 * @brief Frees all memory allocated for the linked list
 * @param head Head of the linked list to free
 */
void freeList(Node *head)
{
  Node *temp;
  while (head != NULL)
  {
    temp = head;
    head = head->next;
    free(temp);
  }
}

/**
 * @brief Main function that demonstrates parallel file processing
 * @return Exit status (0 for success)
 */
int main()
{
  Node *head = NULL;

  // Create a list of sample filenames
  for (int i = 0; i < 10; i++)
  {
    char name[100];
    snprintf(name, sizeof(name), "file%d.txt", i);
    appendNode(&head, name);
  }

  Node *current = head;

#pragma omp parallel
  {
#pragma omp single
    {
      while (current != NULL)
      {
        Node *node = current;

#pragma omp task firstprivate(node)
        {
          int threadId = omp_get_thread_num();
          printf("File: %s (ptr: %p), processed by thread %d\n",
                 node->filename, (void *)node, threadId);
        }

        // #pragma omp critical
        current = current->next;
      }
#pragma omp taskwait
    }
  }

  freeList(head);
  return EXIT_SUCCESS;
}