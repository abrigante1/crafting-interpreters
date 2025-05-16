#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  struct Node *next;
  struct Node *prev;
  int value;
} Node;

typedef enum InsertMode {
  INSERT_BEFORE,
  INSERT_AFTER
} InsertMode;


// Utility Functions

Node *create_node(int val) {
  Node *node = malloc(sizeof(Node));
  assert(node != nullptr);
  node->next = nullptr;
  node->prev = nullptr;
  node->value = val;
  return node;
}

Node *find_head_node(Node *const node) {
  if (node == nullptr) {
    return nullptr;
  }

  // Traverse to List Head, if we are not already there.
  Node *curr_node = node;
  while (curr_node->prev != nullptr) {
    curr_node = node->prev;
  }

  return curr_node;
}

Node *find_node_aux(Node *const head_node, int val) {
  Node *curr_node = head_node;
  if (curr_node == nullptr) {
    return nullptr;
  }

  if (curr_node->value == val) {
    return curr_node;
  }

  return find_node_aux(curr_node->next, val);
}

Node *insert_element_after(Node *const prev_node, int val) {
  assert(prev_node != nullptr);

  // Create Node
  Node *new_node = create_node(val);
  new_node->prev = prev_node;

  // Check if Prev Node Has Next, Cache it
  Node *next_node = prev_node->next;

  // Update prev_node next to new node
  prev_node->next = new_node;

  // Update cached_next node prev to new node.
  if (next_node != nullptr) {
    next_node->prev = new_node;
  }

  return new_node;
}

Node *insert_element_before(Node *const next_node, int val) {
  assert(next_node != nullptr);

  // Create Node
  Node *new_node = create_node(val);
  new_node->next = next_node;
  new_node->prev = next_node->prev;
  
  // Update the next node's prev to the new node
  next_node->prev = new_node;

  // Update cached_next node prev to new node.
  if (new_node->prev != nullptr) {
    new_node->prev->next = new_node;
  }

  return new_node;
}

// List API

Node *find_node(Node *const node, int val) {
  Node *head_node = find_head_node(node);
  if (head_node == nullptr) {
    return nullptr; 
  }

  return find_node_aux(head_node, val);
}

Node *insert_element(Node *const node, int val, InsertMode insert_mode) {
  if (insert_mode == INSERT_AFTER) {
    return insert_element_after(node, val);
  } else if (insert_mode == INSERT_BEFORE) {
    return insert_element_before(node, val);
  }

  return nullptr;
}

bool remove_element(Node *head, int val) {
  Node *val_node = find_node(head, val);
  if (val_node == nullptr) {
    return false;
  }

  // Connect the found node's prev to the val node's next
  if (val_node->prev != nullptr) {
    val_node->prev->next = val_node->next;
  }

  if (val_node->next != nullptr) {
    val_node->next->prev = val_node->prev;
  }

  free(val_node);
  val_node = nullptr;

  return true;
}

void print_node(Node *node) {
  if (node == nullptr) {
    printf(" NULL ");
    return;
  }

  if (node->prev != nullptr) {
    printf(" | %d | ", node->prev->value);
  } else {
    printf("  NULL ");
  }

  printf("<-- | %d | -->", node->value);

  if (node->next != nullptr) {
    printf(" | %d | ", node->next->value);
  } else {
    printf("  NULL ");
  }

  printf("\n");
}

void print_list(Node *node) {
  Node *curr_node = find_head_node(node);

  int spaceCount = 0;
  while (curr_node) {
    printf("%*s", spaceCount, "");
    print_node(curr_node);
    curr_node = curr_node->next;
    spaceCount += 10;
  }

  printf("\n");
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  
  // Create Head Node
  printf(" --- Creating List ---\n");
  Node *head = create_node(1);
  print_list(head);

  // Insert Nodes
  printf(" --- Inserting Nodes After ---\n");
  Node *next_node = insert_element(head, 2, INSERT_AFTER);
  next_node = insert_element(next_node, 4, INSERT_AFTER);
  next_node = insert_element(next_node, 6, INSERT_AFTER);
  print_list(head);

  printf(" --- Inserting Nodes Before ---\n");
  insert_element(find_node(head, 4), 3, INSERT_BEFORE);
  insert_element(find_node(head, 6), 5, INSERT_BEFORE);
  print_list(head);

  printf(" --- Find and Remove Nodes ---\n");
  remove_element(head, 3);
  print_list(head);

  return 0;
}
