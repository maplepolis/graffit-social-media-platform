/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#ifndef __testing
#define MAT_SIZE 3	// A small graph
#endif

typedef struct user_struct {
    char name[MAX_STR_LEN];
    struct friend_node_struct* friends;
    struct brand_node_struct* brands;
    bool visited;
} User;

typedef struct friend_node_struct {
    User* user;
    struct friend_node_struct* next;
} FriendNode;

typedef struct brand_node_struct {
    char brand_name[MAX_STR_LEN];
    struct brand_node_struct* next;
} BrandNode;

/** Note: We are reusing the FriendNode here as a Linked List for allUsers.
  * This is usually bad coding practice but it will allow us to reuse the
  * helper functions.
  **/
FriendNode* allUsers; 

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Checks if a user is inside a FriendNode LL.
 **/
bool in_friend_list(FriendNode *head, User *node) {
  for (FriendNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->user->name, node->name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Checks if a brand is inside a BrandNode LL.
 **/
bool in_brand_list(BrandNode *head, char *name) {
  for (BrandNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->brand_name, name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Inserts a User into a FriendNode LL in sorted position. If the user 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
FriendNode *insert_into_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (in_friend_list(head, node)) {
    printf("User already in list\n");
    return head;
  }
  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->user->name, node->name) > 0) {
    fn->next = head;
    return fn;
  } 

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Inserts a brand into a BrandNode LL in sorted position. If the brand 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
BrandNode *insert_into_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (in_brand_list(head, node)) {
    printf("Brand already in list\n");
    return head;
  }
  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->brand_name, node) > 0) {
    fn->next = head;
    return fn;
  } 

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Deletes a User from FriendNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
FriendNode *delete_from_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (!in_friend_list(head, node)) {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0) {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Deletes a brand from BrandNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
BrandNode *delete_from_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (!in_brand_list(head, node)) {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0) {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Prints out the user data.
 **/
void print_user_data(User *user) {
  printf("User name: %s\n", user->name);
  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next) {
    printf("   %s\n", f->user->name);
  }
  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next) {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Get the index into brand_names for the given brand name. If it doesn't
 * exist in the array, return -1
 **/
int get_brand_index(char *name) {
  for (int i = 0; i < MAT_SIZE; i++) {
    if (strcmp(brand_names[i], name) == 0) {
      return i;
    }
  }
  printf("brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Print out brand name, index and similar brands.
 **/
void print_brand_data(char *brand_name) {
  int idx = get_brand_index(brand_name);
  if (idx < 0) {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }
  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);
  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++) {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0) {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char* file_name) {
    // Read the file
    char buff[MAX_STR_LEN];
    FILE* f = fopen(file_name, "r");
    fscanf(f, "%s", buff);
    char* line = buff;
    // Load up the brand_names matrix
    for (int i = 0; i < MAT_SIZE; i++) {
        if (i == MAT_SIZE - 1) {
            strcpy(brand_names[i], line);
            break;
        }
        int index = strchr(line, ',') - line;
        strncpy(brand_names[i], line, index);
        line = strchr(line, ',') + sizeof(char);
    }
    // Load up the brand_adjacency_matrix
    for (int x = 0; x < MAT_SIZE; x++) {
        fscanf(f, "%s", buff);
        for (int y = 0; y < MAT_SIZE; y++) {
            int value = (int) buff[y*2];
            if (value == 48) { value = 0; }
            else {value = 1;}
            brand_adjacency_matrix[x][y] = value;
        }
    }
}


// Users
/**TODO: Complete this function
 * Creates and returns a user. Returns NULL on failure.
 **/

void print_allUsers()
{
	FriendNode* p = allUsers;
	while(p != NULL)
	{
		print_user_data(p->user);
		p = p->next;
	}
}

void print_matrix()
{
	for(int i = 0; i < MAT_SIZE; i++)
	{
		for(int j = 0; j < MAT_SIZE; j++)
		{
			printf("%d ", brand_adjacency_matrix[i][j]);
		}
		printf("\n");
	}
}

FriendNode* insert_into_allUsers(FriendNode* head, User *user)
{
	if(head == NULL)
	{
		FriendNode* p = (FriendNode*)calloc(1, sizeof(FriendNode));
		p->user = user;
		p->next = NULL;
		return p;
	}
	else
	{
		FriendNode* q = (FriendNode*)calloc(1, sizeof(FriendNode));
		q->user = user;
		q->next = head;
		return q;
	}
}

User* create_user(char* name)
{
	if(name == NULL)
		return NULL;
	if(strlen(name) > MAX_STR_LEN)
		return NULL;
	if(strcmp(name, "") == 0)
		return NULL;
	User *new_user = (User*)calloc(1, sizeof(User));
	new_user->friends = NULL;
	new_user->brands = NULL;
	strcpy(new_user->name, name);
	new_user->visited = false;
	
	if(!in_friend_list(allUsers, new_user))
		allUsers = insert_into_allUsers(allUsers, new_user);
	return new_user;
}

/**TODO: Complete this function
 * Deletes a given user. 
 * Returns 0 on success, -1 on failure.
 **/
int delete_user(User* user)
{
	if(user == NULL)
		return -1;
	if(!in_friend_list(allUsers, user))
		return -1;
	
	
	FriendNode* user_friend_list = user->friends;
	while(user_friend_list != NULL)
	{
		user_friend_list->user->friends= delete_from_friend_list(user_friend_list->user->friends, user);
		user_friend_list = user_friend_list->next;
	}
	
	allUsers = delete_from_friend_list(allUsers, user);
	free(user);
	return 0;
}

/**TODO: Complete this function
 * Create a friendship between user and friend.
 * Returns 0 on success, -1 on failure.
 **/
int add_friend(User* user, User* friend)
{
	if(user == NULL || friend == NULL)
		return -1;
	if(in_friend_list(user->friends, friend))
		return -1;
	user->friends = insert_into_friend_list(user->friends, friend);
	friend->friends = insert_into_friend_list(friend->friends, user);
	return 0;
}

/**TODO: Complete this function
 * Removes a friendship between user and friend.
 * Returns 0 on success, -1 on faliure.
 **/
int remove_friend(User* user, User* friend)
{
	if(user == NULL || friend == NULL)
		return -1;
	if(!in_friend_list(user->friends, friend))
		return -1;
	user->friends = delete_from_friend_list(user->friends, friend);
	friend->friends = delete_from_friend_list(friend->friends, user);
	return 0;
}

/**TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int follow_brand(User* user, char* brand_name)
{
	if(user == NULL || brand_name == NULL)
		return -1;
	if(in_brand_list(user->brands, brand_name))
		return -1;
	
	user->brands = insert_into_brand_list(user->brands, brand_name);
	return 0;
	
}

/**TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int unfollow_brand(User* user, char* brand_name)
{
	if(user == NULL || brand_name == NULL)
		return -1;
	if(!in_brand_list(user->brands, brand_name))
		return -1;
	
	user->brands = delete_from_brand_list(user->brands, brand_name);
	return 0;
}

/**TODO: Complete this function
 * Return the number of mutual friends between two users.
 **/
int get_mutual_friends(User* a, User* b)
{
	if(a == NULL || b == NULL)
		return 0;
	
	int count = 0;
	
	FriendNode* a_friend_list = a->friends;
	while(a_friend_list != NULL)
	{
		if(in_friend_list(b->friends, a_friend_list->user))
			count++;
		a_friend_list = a_friend_list->next;
	}
	
	return count;
}

/**TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another.
 * 
 * For example, if X & Y are friends, then we expect to recieve 1 when calling
 * this on (X,Y). Continuing on, if Y & Z are friends, then we expect to
 * recieve 2 when calling this on (X,Z).
 * 
 * Returns a non-negative integer representing the degrees of connection
 * between two users, -1 on failure.
 **/

void clear_visited()
{
	FriendNode* p = allUsers;
	while(p != NULL)
	{
		p->user->visited = false;
		p = p->next;
	}
}

typedef struct user_distance {
    User* user;
	int distance;
    struct user_distance* next;
} UserDistance;

void changeDistance(UserDistance* head, User* user, int distance)
{
	UserDistance *p = head;
	while(p != NULL)
	{
		if(p->user == user)
			p->distance = distance;
		p = p->next;
	}
}

UserDistance* add_user_to_userdistance(UserDistance* head, User* user)
{
	UserDistance* new_node = (UserDistance*)calloc(1, sizeof(UserDistance));
	new_node->user = user;
	new_node->distance = 9999;
	new_node->next = head;
	
	return new_node;
}

int get_distance(UserDistance* head, User* user)
{
	UserDistance *p = head;
	while(p != NULL)
	{
		if(p->user == user)
			return p->distance;
		p = p->next;
	}
	return -1;
}

/*FriendNode* remove_from_queue(FriendNode* head, User *user)
{
	user = head->user;
	FriendNode* new_head = head->next;
	free(head);
	return new_head;
	
}*/

int get_degrees_of_connection(User* a, User* b)
{
	if(a == NULL || b == NULL)
		return -1;
	if(a == b)
		return 0;
	
	UserDistance* distance_list = NULL;
	FriendNode* vertex_set = NULL;
	
	FriendNode* p = allUsers;
	//printf("%p\n", allUsers);
	while(p != NULL)
	{
		distance_list = add_user_to_userdistance(distance_list, p->user);
		//printf("%s : %d\n", p->user->name, get_distance(distance_list, p->user));
		p = p->next;
	}
	
	vertex_set = insert_into_allUsers(vertex_set, a);
	changeDistance(distance_list, a, 0);
	//printf("%s : %d\n", a->name, get_distance(distance_list, a));
	//printf("%s\n", vertex_set->user->name);
	while(vertex_set != NULL)
	{
		User* u = vertex_set->user;
		FriendNode* temp = vertex_set->next;
		free(vertex_set);
		vertex_set = temp;
		int distanceU = get_distance(distance_list, u);
		if(!u->visited)
		{
			u->visited = true;
			FriendNode* friend_list = u->friends;
			while(friend_list != NULL)
			{
				if(distanceU + 1 < get_distance(distance_list, friend_list->user))
					changeDistance(distance_list, friend_list->user, distanceU + 1);
				
				vertex_set = insert_into_allUsers(vertex_set, friend_list->user);
				friend_list = friend_list->next;
			}
		}
	}
	
	clear_visited();
	//printf("hello\n");
	//printf("%d\n", get_distance(distance_list, b));
	
	if(get_distance(distance_list, b) == 9999)
		return -1;
	return get_distance(distance_list, b);
}

/*int count = 1;
int get_degrees_of_connection(User* a, User* b)
{
	if(a == NULL || b == NULL)
		return -1;
	if(a == b)
		return 0;
	
	printf("hi\n");
	if(in_friend_list(a->friends, b))
	{
		printf("hi\n");
		return count;
	}
	else
	{
		count++;
		FriendNode* p = a->friends;
		while(p != NULL)
		{
			get_degrees_of_connection(p->user, b);
			p = p->next;
		}
	}
}*/

// Brands
/**TODO: Complete this function
 * Marks two brands as similar.
 **/
void connect_similar_brands(char* brandNameA, char* brandNameB)
{
	if(brandNameA == NULL || brandNameB == NULL)
		return;
	
	int i = get_brand_index(brandNameA);
	int j = get_brand_index(brandNameB);
	
	if(i == -1 || j == -1)
		return;
	brand_adjacency_matrix[i][j] = 1;
	brand_adjacency_matrix[j][i] = 1;
}

/**TODO: Complete this function
 * Marks two brands as not similar.
 **/
void remove_similar_brands(char* brandNameA, char* brandNameB)
{
	if(brandNameA == NULL || brandNameB == NULL)
		return;
	
	int i = get_brand_index(brandNameA);
	int j = get_brand_index(brandNameB);
	
	if(i == -1 || j == -1)
		return;
	
	brand_adjacency_matrix[i][j] = 0;
	brand_adjacency_matrix[j][i] = 0;
}


// Harder ones
/**TODO: Complete this function
 * Returns a suggested friend for the given user, returns NULL on failure.
 * See the handout for how we define a suggested friend.
 **/

/*void lowercase(char *name)
{
	for(int i = 0;i < strlen(name); i++)
	{
		if(name[i]>=65 && name[i] <= 90)
			name[i] = name[i] + 32;
	}
}*/

typedef struct users_with_same_brands
{
	User* user;
	int num_brands;
	struct users_with_same_brands *next;
}UWSB;

UWSB* add_to_uwsb(UWSB* head, User* user, int num_brands)
{
	UWSB *new_node = (UWSB*)calloc(1, sizeof(UWSB));
	new_node->user = user;
	new_node->num_brands = num_brands;
	new_node->next = NULL;
	
	if(head == NULL)
	{
		head = new_node;
		return head;
	}
	else
	{
		new_node->next = head;
		return new_node;
	}
	
	return head;
}

int get_num_similiar_brands(User *a, User *b)
{
	int count = 0;
	BrandNode *a_brands = a->brands;
	while(a_brands != NULL)
	{
		if(in_brand_list(b->brands, a_brands->brand_name))
			count++;
		a_brands = a_brands->next;
	}
	
	return count;
}

UWSB* initialize_same_brands(UWSB* head, User* og_user)
{
	FriendNode* all_the_users = allUsers;
	while(all_the_users != NULL)
	{
		if(!in_friend_list(og_user->friends, all_the_users->user) && og_user != all_the_users->user)
			head = add_to_uwsb(head, all_the_users->user, get_num_similiar_brands(og_user, all_the_users->user));
		all_the_users = all_the_users->next;
	}
	
	return head;
}

User* highest_same_brands_count(UWSB* head)
{
	if(head == NULL)
		return NULL;
	
	int highest = head->num_brands;
	User* recommended = head->user;
	UWSB* p = head;
	while(p != NULL)
	{
		if(p->num_brands > highest)
		{
			highest = p->num_brands;
			recommended = p->user;
		}
		else if(p->num_brands == highest)
		{
			char recommended_name_copy[MAX_STR_LEN];
			strcpy(recommended_name_copy, recommended->name);
			char user_name_copy[MAX_STR_LEN];
			strcpy(user_name_copy, p->user->name);
			//lowercase(recommended_name_copy);
			//lowercase(user_name_copy);
			
			//printf("%s\n", user_name_copy);
			//printf("%s\n", recommended_name_copy);
			if(strcmp(recommended_name_copy, user_name_copy) < 0)
			{
				highest = p->num_brands;
				recommended = p->user;
			}
		}
		p = p->next;
	}
	
	return recommended;
}

User* get_suggested_friend(User* user)
{
	UWSB* the_list = NULL;
	the_list = initialize_same_brands(the_list, user);
	
	UWSB *p = the_list;
	
	/*print_allUsers();
	while(p != NULL)
	{
		print_user_data(p->user);
		printf("%d\n", p->num_brands);
		p = p->next;
	}*/
	
	User* recommended = highest_same_brands_count(the_list);
	
	//print_user_data(recommended);
	
	return recommended;
	
}

/**TODO: Complete this function
 * Friends n suggested friends for the given user.
 * See the handout for how we define a suggested friend.
 * Returns how many friends were successfully followed.
 **/
int add_suggested_friends(User* user, int n)
{
	FriendNode* suggested_friends = NULL;
	
	int count = 0;
	for(int i = 0; i < n; i++)
	{
		User* recommended = get_suggested_friend(user);
		if(recommended != NULL)
		{
			add_friend(user, recommended);
			count++;
		}
	}
	
	return count;
}

/**TODO: Complete this function
 * Follows n suggested brands for the given user.
 * See the handout for how we define a suggested brand.     
 * Returns how many brands were successfully followed. 	  	
 **/
 
bool is_similiar(char* brandNameA, char* brandNameB)
{
	int i = get_brand_index(brandNameA);
	int j = get_brand_index(brandNameB);
	
	if(brand_adjacency_matrix[i][j] == 1)
		return true;
	else
		return false;
}


int follow_suggested_brands(User* user, int n)
{
	if(user == NULL)
		return 0;
	int success = 0;
	int similarity[MAT_SIZE];
	for(int t = 0; t< MAT_SIZE; t++)
		similarity[t] = 0;
	
	for(int i = 0; i < MAT_SIZE; i++)
	{
		BrandNode *b = user->brands;
		while(b != NULL)
		{
			if(is_similiar(brand_names[i], b->brand_name))
			{
				similarity[i] = similarity[i] + 1;
			}
				
			b = b->next;
		}
	}
	
	for(int j = 0; j < n; j++)
	{
		char recommended[MAX_STR_LEN];
		int highest = 1;
		bool did_it = false;
		
		for(int k = 0; k < MAT_SIZE; k++)
		{
			if(similarity[k] > highest)
			{
				highest = similarity[k];
				strcpy(recommended, brand_names[k]);
				did_it = true;
			}
			else if(similarity[k] == highest)
			{
				char recommended_copy[MAX_STR_LEN];
				char brand_copy[MAX_STR_LEN];
				strcpy(recommended_copy, recommended);
				strcpy(brand_copy, brand_names[k]);
				
				//printf("%s\n", recommended_copy);
				//printf("%s\n", brand_copy);
				
				if(strcmp(recommended_copy, brand_copy) <= 0)
				{
					//printf("hi\n");
					highest = similarity[k];
					strcpy(recommended, brand_names[k]);
					did_it = true;
				}
			}
		}
		
		if(did_it)
		{
			//printf("---------------\n");
			//printf("%s\n", recommended);
			similarity[get_brand_index(recommended)] = 0;
			follow_brand(user, recommended);
			success++;
		}
	}
	
	return success;
}