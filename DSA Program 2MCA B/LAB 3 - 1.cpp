#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Domain: Task Scheduling System
typedef struct Task {
    int priority;        // Key for tree operations
    char name[50];
    int duration;        // in minutes
} Task;

// BST Node
typedef struct BSTNode {
    Task data;
    struct BSTNode *left, *right;
} BSTNode;

// AVL Node (with height)
typedef struct AVLNode {
    Task data;
    int height;
    struct AVLNode *left, *right;
} AVLNode;

// Global operation counters for complexity analysis
int bst_ops = 0, avl_ops = 0;

// ============= BST OPERATIONS =============
BSTNode* createBSTNode(Task t) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->data = t;
    node->left = node->right = NULL;
    return node;
}

BSTNode* insertBST(BSTNode* root, Task t) {
    bst_ops++;
    if (!root) return createBSTNode(t);
    
    if (t.priority < root->data.priority)
        root->left = insertBST(root->left, t);
    else if (t.priority > root->data.priority)
        root->right = insertBST(root->right, t);
    else
        printf("Priority %d already exists!\n", t.priority);
    
    return root;
}

BSTNode* minValueNode(BSTNode* node) {
    while (node->left) node = node->left;
    return node;
}

BSTNode* deleteBST(BSTNode* root, int key) {
    if (!root) return root;
    bst_ops++;
    
    if (key < root->data.priority)
        root->left = deleteBST(root->left, key);
    else if (key > root->data.priority)
        root->right = deleteBST(root->right, key);
    else {
        if (!root->left) {
            BSTNode* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            BSTNode* temp = root->left;
            free(root);
            return temp;
        }
        BSTNode* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data.priority);
    }
    return root;
}

BSTNode* searchBST(BSTNode* root, int key) {
    bst_ops++;
    if (!root || root->data.priority == key) return root;
    if (key < root->data.priority) return searchBST(root->left, key);
    return searchBST(root->right, key);
}

// ============= AVL OPERATIONS =============
int height(AVLNode* n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* createAVLNode(Task t) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->data = t;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    
    return y;
}

int getBalance(AVLNode* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

AVLNode* insertAVL(AVLNode* node, Task t) {
    avl_ops++;
    if (!node) return createAVLNode(t);
    
    if (t.priority < node->data.priority)
        node->left = insertAVL(node->left, t);
    else if (t.priority > node->data.priority)
        node->right = insertAVL(node->right, t);
    else {
        printf("Priority %d already exists!\n", t.priority);
        return node;
    }
    
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    
    // Left-Left
    if (balance > 1 && t.priority < node->left->data.priority)
        return rightRotate(node);
    
    // Right-Right
    if (balance < -1 && t.priority > node->right->data.priority)
        return leftRotate(node);
    
    // Left-Right
    if (balance > 1 && t.priority > node->left->data.priority) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Right-Left
    if (balance < -1 && t.priority < node->right->data.priority) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node;
}

AVLNode* minValueNodeAVL(AVLNode* node) {
    while (node->left) node = node->left;
    return node;
}

AVLNode* deleteAVL(AVLNode* root, int key) {
    if (!root) return root;
    avl_ops++;
    
    if (key < root->data.priority)
        root->left = deleteAVL(root->left, key);
    else if (key > root->data.priority)
        root->right = deleteAVL(root->right, key);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            AVLNode* temp = minValueNodeAVL(root->right);
            root->data = temp->data;
            root->right = deleteAVL(root->right, temp->data.priority);
        }
    }
    
    if (!root) return root;
    
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);
    
    // Left-Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    
    // Left-Right
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    
    // Right-Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    
    // Right-Left
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    
    return root;
}

AVLNode* searchAVL(AVLNode* root, int key) {
    avl_ops++;
    if (!root || root->data.priority == key) return root;
    if (key < root->data.priority) return searchAVL(root->left, key);
    return searchAVL(root->right, key);
}

// ============= DISPLAY OPERATIONS =============
void inorderBST(BSTNode* root) {
    if (root) {
        inorderBST(root->left);
        printf("P%d: %s (%dm) | ", root->data.priority, root->data.name, root->data.duration);
        inorderBST(root->right);
    }
}

void preorderBST(BSTNode* root) {
    if (root) {
        printf("P%d: %s (%dm) | ", root->data.priority, root->data.name, root->data.duration);
        preorderBST(root->left);
        preorderBST(root->right);
    }
}

void postorderBST(BSTNode* root) {
    if (root) {
        postorderBST(root->left);
        postorderBST(root->right);
        printf("P%d: %s (%dm) | ", root->data.priority, root->data.name, root->data.duration);
    }
}

void inorderAVL(AVLNode* root) {
    if (root) {
        inorderAVL(root->left);
        printf("P%d: %s (%dm) | ", root->data.priority, root->data.name, root->data.duration);
        inorderAVL(root->right);
    }
}

void preorderAVL(AVLNode* root) {
    if (root) {
        printf("P%d: %s (%dm) | ", root->data.priority, root->data.name, root->data.duration);
        preorderAVL(root->left);
        preorderAVL(root->right);
    }
}

void postorderAVL(AVLNode* root) {
    if (root) {
        postorderAVL(root->left);
        postorderAVL(root->right);
        printf("P%d: %s (%dm) | ", root->data.priority, root->data.name, root->data.duration);
    }
}

// ============= BST MENU =============
void bstMenu() {
    BSTNode* root = NULL;
    int choice, priority, duration;
    char name[50];
    
    while (1) {
        printf("\n--- BST Task Scheduler ---\n");
        printf("1. Insert Task\n2. Delete Task\n3. Search Task\n");
        printf("4. Display (Inorder)\n5. Display (Preorder)\n6. Display (Postorder)\n");
        printf("7. Back to Main Menu\nChoice: ");
        scanf("%d", &choice);
        
        bst_ops = 0;
        switch (choice) {
            case 1:
                printf("Priority (1-100): ");
                scanf("%d", &priority);
                printf("Task Name: ");
                scanf(" %[^\n]", name);
                printf("Duration (min): ");
                scanf("%d", &duration);
                Task t = {priority, "", duration};
                strcpy(t.name, name);
                root = insertBST(root, t);
                printf("Inserted! Operations: %d\n", bst_ops);
                break;
            case 2:
                printf("Priority to delete: ");
                scanf("%d", &priority);
                root = deleteBST(root, priority);
                printf("Deleted! Operations: %d\n", bst_ops);
                break;
            case 3:
                printf("Priority to search: ");
                scanf("%d", &priority);
                BSTNode* found = searchBST(root, priority);
                if (found)
                    printf("Found: %s (%dm) | Ops: %d\n", found->data.name, found->data.duration, bst_ops);
                else
                    printf("Not found! Operations: %d\n", bst_ops);
                break;
            case 4:
                printf("Inorder: ");
                inorderBST(root);
                printf("\n");
                break;
            case 5:
                printf("Preorder: ");
                preorderBST(root);
                printf("\n");
                break;
            case 6:
                printf("Postorder: ");
                postorderBST(root);
                printf("\n");
                break;
            case 7:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// ============= AVL MENU =============
void avlMenu() {
    AVLNode* root = NULL;
    int choice, priority, duration;
    char name[50];
    
    while (1) {
        printf("\n--- AVL Task Scheduler ---\n");
        printf("1. Insert Task\n2. Delete Task\n3. Search Task\n");
        printf("4. Display (Inorder)\n5. Display (Preorder)\n6. Display (Postorder)\n");
        printf("7. Back to Main Menu\nChoice: ");
        scanf("%d", &choice);
        
        avl_ops = 0;
        switch (choice) {
            case 1:
                printf("Priority (1-100): ");
                scanf("%d", &priority);
                printf("Task Name: ");
                scanf(" %[^\n]", name);
                printf("Duration (min): ");
                scanf("%d", &duration);
                Task t = {priority, "", duration};
                strcpy(t.name, name);
                root = insertAVL(root, t);
                printf("Inserted! Operations: %d\n", avl_ops);
                break;
            case 2:
                printf("Priority to delete: ");
                scanf("%d", &priority);
                root = deleteAVL(root, priority);
                printf("Deleted! Operations: %d\n", avl_ops);
                break;
            case 3:
                printf("Priority to search: ");
                scanf("%d", &priority);
                AVLNode* found = searchAVL(root, priority);
                if (found)
                    printf("Found: %s (%dm) | Ops: %d\n", found->data.name, found->data.duration, avl_ops);
                else
                    printf("Not found! Operations: %d\n", avl_ops);
                break;
            case 4:
                printf("Inorder: ");
                inorderAVL(root);
                printf("\n");
                break;
            case 5:
                printf("Preorder: ");
                preorderAVL(root);
                printf("\n");
                break;
            case 6:
                printf("Postorder: ");
                postorderAVL(root);
                printf("\n");
                break;
            case 7:
                return;
            default:
                printf("Invalid choice!\n");
        }
    }
}

// ============= COMPARISON =============
void compareComplexity() {
    printf("\n========== TIME COMPLEXITY COMPARISON ==========\n\n");
    
    printf("BINARY SEARCH TREE (BST):\n");
    printf("+- Insert:  Average O(log n) | Worst O(n) [skewed tree]\n");
    printf("+- Delete:  Average O(log n) | Worst O(n)\n");
    printf("+- Search:  Average O(log n) | Worst O(n)\n");
    printf("+- Issue:   Becomes unbalanced with sorted data\n\n");
    
    printf("AVL TREE (Self-Balancing):\n");
    printf("+- Insert:  Always O(log n) [guaranteed]\n");
    printf("+- Delete:  Always O(log n) [guaranteed]\n");
    printf("+- Search:  Always O(log n) [guaranteed]\n");
    printf("+- Benefit: Height-balanced via rotations\n\n");
    
    printf("PRACTICAL INSIGHT:\n");
    printf("• BST is faster for random insertions (no rotation overhead)\n");
    printf("• AVL guarantees O(log n) for sorted/sequential data\n");
    printf("• AVL ideal for search-heavy applications (cloud task scheduling)\n\n");
    
    printf("LEARNING CONCEPTS:\n");
    printf("1. Tree Rotations: LL, RR, LR, RL for AVL balancing\n");
    printf("2. Balance Factor: |height(left) - height(right)| = 1\n");
    printf("3. In-order traversal gives sorted task priorities\n");
    printf("4. Space Complexity: O(n) for both trees\n");
    printf("================================================\n");
}

// ============= MAIN =============
int main() {
    int choice;
    
    printf("====================================\n");
    printf("  CLOUD TASK SCHEDULER (BST & AVL)\n");
    printf("====================================\n");
    
    while (1) {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Work with BST\n");
        printf("2. Work with AVL Tree\n");
        printf("3. Compare Time Complexities\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                bstMenu();
                break;
            case 2:
                avlMenu();
                break;
            case 3:
                compareComplexity();
                break;
            case 4:
                printf("Exiting... Tasks scheduled successfully!\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
