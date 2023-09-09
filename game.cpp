#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

int totalData = 0;

struct Node{
	char title[30];
	char genre[10];
	int stock;
	
	int height, bf;
	Node *left, *right;
};

Node *createNewNode(const char *title, const char *genre, int stock){
	Node *newNode = (Node*)malloc(sizeof(Node));
	strcpy(newNode->title, title);
	strcpy(newNode->genre, genre);
	newNode->stock = stock;
	
	newNode->height = 1;
	newNode->bf = 0;
	newNode->left = newNode->right = NULL;
	
	return newNode;
}

int getHeight(Node *root){
	if(root == NULL) return 0;
	else return root->height;
}

int maxHeight(int a, int b){
	return(a>b) ? a:b;
}

int computeNewHeight(Node *root){
	return maxHeight(getHeight(root->left), getHeight(root->right)) + 1;
}

Node *updateNode(Node *root){
	root->height = computeNewHeight(root);
	root->bf = getHeight(root->left) - getHeight(root->right);
	
	return root;
}

Node *leftRotate(Node *root){
	Node *pivot = root->right;
	Node *temp = pivot->left;
	
	pivot->left = root;
	root->right = temp;
	
	root = updateNode(root);
	pivot = updateNode(pivot);
	
	return pivot;
}

Node *rightRotate(Node *root){
	Node *pivot = root->left;
	Node *temp = pivot->right;
	
	pivot->right = root;
	root->left = temp;
	
	root = updateNode(root);
	pivot = updateNode(pivot);
	
	return pivot;
}

Node *rotation(Node *root){
	if(root->bf < -1 && root->right->bf < 0){
		return leftRotate(root);
	} else if(root->bf < -1 && root->right->bf > 0){
		root->right = rightRotate(root->right);
		return leftRotate(root);
	} else if(root->bf > 1 && root->left->bf > 0){
		return rightRotate(root);
	} else if(root->bf > 1 && root->left->bf < 0){
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}
	return root;
}

Node *insert(Node *root, const char *title, const char *genre, int stock){
	if(root == NULL){
		return createNewNode(title, genre, stock);
	} else if(strcmp(title, root->title) < 0){
		root->left = insert(root->left, title, genre, stock);
	} else if(strcmp(title, root->title) > 0){
		root->right = insert(root->right, title, genre, stock);
	}
	return rotation(updateNode(root));
}

Node *predecessor(Node *root){
	Node *temp = root;
	
	if(temp && temp->right != NULL){
		temp = temp->right;
	}
	return temp;
}

Node *add(Node *root, const char *title, int quantity) {
    if (root == NULL) {
        puts("Data not found!");
        puts("Press enter to continue...");
    } else if (strcmp(title, root->title) < 0) {
        root->left = add(root->left, title, quantity);
    } else if (strcmp(title, root->title) > 0) {
        root->right = add(root->right, title, quantity);
    } else {
        root->stock += quantity;
        puts("Stock updated successfully!");
        printf("%d %s added to the warehouse\n", quantity, title);
        puts("Press enter to continue...");
    }
    return root;
}

Node *del(Node *root, const char *title, int quantity) {
    if (root == NULL) {
        puts("Data not found!");
        puts("Press enter to continue...");
        return NULL;
    } else if (strcmp(title, root->title) < 0) {
        root->left = del(root->left, title, quantity);
    } else if (strcmp(title, root->title) > 0) {
        root->right = del(root->right, title, quantity);
    } else {
        if (quantity >= root->stock) {
            quantity = root->stock;  // Remove available stock
            root->stock -= quantity;  // Decrease stock by quantity
            puts("Stock updated successfully!");
            printf("%d %s removed from the warehouse\n", quantity, title);
			totalData--;
        } else {
            root->stock -= quantity;  // Decrease stock by quantity
            puts("Stock updated successfully!");
            printf("%d %s removed from the warehouse\n", quantity, title);
        }

        puts("Press enter to continue...");
    }
    return root;
}


void preOrder(Node *root){
	if(root == NULL){
		return;
	}
	printf("|%-30s|%-13s|%-9d|\n", root->title, root->genre, root->stock);
	preOrder(root->left);
	preOrder(root->right);
}

void inOrder(Node *root){
	if(root == NULL){
		return;
	}
	inOrder(root->left);
	printf("|%-30s|%-13s|%-9d|\n", root->title, root->genre, root->stock);
	inOrder(root->right);
}

void postOrder(Node *root){
	if(root == NULL){
		return;
	}
	postOrder(root->left);
	postOrder(root->right);
	printf("|%-30s|%-13s|%-9d|\n", root->title, root->genre, root->stock);
}


int main(){
	Node *root = NULL;
	int opt, len;
	
	do{
		system("cls");
		puts("Bluejack Gshop");
		puts("==============");
		puts("1. Insert Game");
		puts("2. View Game");
		puts("3. Update Stock");
		puts("4. Exit");
		printf(">> ");
		
		scanf("%d", &opt); getchar();
		
		switch(opt){
			
			case 1:{
				char judul[30];
				do{
					printf("Input game title [5-25]: ");
					scanf("%[^\n]", judul); getchar();
					
					len = strlen(judul);
				} while(!(len>=5 && len<=25));
				
				char tipe[10];
				do{
					printf("Input game type [Action | RPG | Adventure | Card Game]: ");
					scanf("%[^\n]", tipe); getchar();
				} while(!(strcmp(tipe, "Action")==0 || strcmp(tipe, "RPG")==0 || strcmp(tipe, "Adventure")==0 || strcmp(tipe, "Card Game")==0));
				
				int stok;
				do{
					printf("Input game stock [>=1]: ");
					scanf("%d", &stok); getchar();
				} while(stok<1);
				
				root = insert(root, judul, tipe, stok);
				puts("Insert Success!");
				puts("Press enter to continue...");
				totalData++;
				
				break;
			}
			
			case 2:{
				if(totalData ==0){
					puts("Warehouse is empty!");
					puts("Press enter to continue...");
				} else{
					puts("--------------------------------------------------------");
					puts("| Game Title                   | Game Genre  | Stock   |");
					puts("--------------------------------------------------------");
					inOrder(root);
					puts("--------------------------------------------------------");
				}
				break;
			}
			case 3: {
			    if (totalData == 0) {
			        puts("Warehouse is empty!");
			        puts("Press enter to continue...");
			    } else {
			        char title[30];
			        char update[10];
			        int stockUpdate;
			        
			        puts("--------------------------------------------------------");
			        puts("| Game Title                   | Game Genre  | Stock   |");
			        puts("--------------------------------------------------------");
			        inOrder(root);
			        puts("--------------------------------------------------------");
			        
			        printf("Input game title: ");
			        scanf("%[^\n]", title); getchar();
			        
			        printf("Input update type [add | remove]: ");
			        scanf("%[^\n]", update); getchar();
			        
			        printf("Input stock to %s: ", update);
			        scanf("%d", &stockUpdate); getchar();
			        
			        if (strcmp(update, "add") == 0) {
			            root = add(root, title, stockUpdate);
			            totalData++;
			        } else if (strcmp(update, "remove") == 0) {
			            root = del(root, title, stockUpdate);
			        } else {
			            puts("Invalid update type!");
			        }
			    }
			    break;
			}
		}
		getchar();	
	} while(opt !=4);	
	return 0;
}
