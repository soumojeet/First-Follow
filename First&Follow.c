#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct rightNode{
	char rname[10];
	struct rightNode* rnext;
};

struct production{
	char lname;
	struct rightNode* rnode;
	
	char first[15];
	int firstCount;
	int isFirstCalculated;
	
	char follow[15];
	int followCount;
	int isFollowCalculated;
	
	struct production* prodNext;
};

struct production* productionFront = NULL;
struct production* productionRear = NULL;
struct production* productionNow = NULL;

struct rightNode* rightTop = NULL;

int isPresent(char* string, char ch){
	int i = 0;
	while(string[i] != '\0'){
		if(string[i] == ch) return 1;
		i++;
	}
	return 0;
}

struct production* isTerminal(char ch){
	struct production* tempProd = productionFront;
	while(tempProd != NULL){
		if(tempProd->lname == ch) return tempProd;
		tempProd = tempProd->prodNext;
	}
	return NULL;
}

void calculatingFirst(struct production* tempProd){
	if(!tempProd->isFirstCalculated){
		tempProd->isFirstCalculated = 1;
		struct rightNode* tempNode = tempProd->rnode;
		while(tempNode != NULL){
			int i, count = 0;
			while(1){
				int flag = 0;
				struct production* tempProd2 = isTerminal(tempNode->rname[count]);
				if(tempProd2 != NULL){
					calculatingFirst(tempProd2);
					for(i=0;i<tempProd2->firstCount;i++){
						if(tempProd2->first[i] == '#' && tempNode->rname[count+1] != '\0'){
							count++; flag = 1;
						} else if(!isPresent(tempProd->first, tempProd2->first[i])){
							strncat(tempProd->first, tempProd2->first+i, 1);
							tempProd->firstCount++;
						}
					}
					if(!flag) break;
				} else {
					if(tempNode->rname[count] == '#' && tempNode->rname[count+1] != '\0'){
							count++; flag = 1;
					} else if(!isPresent(tempProd->first, tempNode->rname[count])){
						strncat(tempProd->first, tempNode->rname+count, 1);
						tempProd->firstCount++;
					}
					if(!flag) break;
				}
			}
		tempNode = tempNode->rnext;
		}
	}
}

void calculatingFollow(struct production* tempProd){
	if(!tempProd->isFollowCalculated){
		tempProd->isFollowCalculated = 1;
		int i, j;
		struct production* tempProd2 = productionFront;
		while(tempProd2 != NULL){
			struct rightNode* tempNode = tempProd2->rnode;
			while(tempNode != NULL){
				for(i=0;i<strlen(tempNode->rname);i++){
					if(tempProd->lname == tempNode->rname[i]){
						int count = 1;
						char temp[15] = "";
						while(1){
							if(tempNode->rname[i+count] == '\0'){
								calculatingFollow(tempProd2);
								for(j=0;j<tempProd2->followCount;j++){
									if(!isPresent(tempProd->follow, tempProd2->follow[j])){
										strncat(tempProd->follow, tempProd2->follow+j, 1);
										tempProd->followCount++;
									}
								}
								break;
							} else {
								int flag = 0;
								struct production* tempProd3 = isTerminal(tempNode->rname[i+count]);
								if(tempProd3 != NULL){
									for(j=0;j<tempProd3->firstCount;j++){
										if(tempProd3->first[j] == '#'){
											count++; flag = 1;
										} else if(!isPresent(tempProd->follow, tempProd3->first[j])){
											strncat(tempProd->follow, tempProd3->first+j, 1);
											tempProd->followCount++;
										}
									}
									if(!flag) break;
								} else {
									if(tempNode->rname[i+count] == '#'){
										count++; flag = 1;
									} else if(!isPresent(tempProd->follow, tempNode->rname[i+count])){
										strncat(tempProd->follow, tempNode->rname+i+count, 1);
										tempProd->followCount++;
									}
									if(!flag) break;
								}
							}
						}
					}
				}
				tempNode = tempNode->rnext;
			}
			tempProd2 = tempProd2->prodNext;
		}
	}
}

int main(){
	int size;
	char input[10];

	printf("Enter Size: ");
	scanf("%d",&size);
	fflush(stdin);

	{//Taking Grammer For Input
		int i;
		printf("Enter Production:\n");
		while(size--){
			scanf("%s",input);
	
			productionNow = (struct production*)malloc(sizeof(struct production));
			productionNow->rnode = NULL;
			productionNow->prodNext = NULL;
			
			productionNow->first[0] = '\0';
			productionNow->follow[0] = '\0';
			
			productionNow->firstCount = 0;
			productionNow->followCount = 0;
			
			productionNow->isFirstCalculated = 0;
			productionNow->isFollowCalculated = 0;
			
			rightTop = (struct rightNode*)malloc(sizeof(struct rightNode));
			rightTop->rnext = NULL;
			
			//Collecting Terminals
			productionNow->lname = input[0];
			
			//Collecting Non-Terminals
			for(i=2;i<=strlen(input);i++) rightTop->rname[i-2] = input[i];
			
			struct production* tempProd = isTerminal(productionNow->lname);
	
			if(tempProd == NULL){
				productionNow->rnode = rightTop;
				if(productionFront == NULL && productionRear == NULL){
					productionFront = productionRear = productionNow;	
				} else {
					productionRear->prodNext = productionNow;
					productionRear = productionRear->prodNext;
				}
			} else {
				struct rightNode* tempNode = tempProd->rnode;
				tempProd->rnode = rightTop;
				tempProd->rnode->rnext = tempNode;
			}	
		}
	//	system("cls");
	}

	{//Calculating Firsts
		struct production* tempProd = productionFront;
		while(tempProd != NULL){
			calculatingFirst(tempProd);
			tempProd = tempProd->prodNext;
		}
	}

	productionFront->follow[0] = '$';
	productionFront->follow[1] = '\0';
	productionFront->followCount = 1;
	
	{//Calculating Follow
		struct production* tempProd = productionFront;
		while(tempProd != NULL){
			calculatingFollow(tempProd);
			tempProd = tempProd->prodNext;
		}
	}
	
	{//Printing the Grammer
		int i;
		struct production* tempProd = productionFront;
		while(tempProd != NULL){
			//Printing eft Side
			printf("%c->",tempProd->lname);
			struct rightNode* tempNode = tempProd->rnode;
			//Printing Right Side
			while(tempNode != NULL){
				printf(" %s",tempNode->rname);
				tempNode = tempNode->rnext;
				if(tempNode != NULL) printf(" |");
			}
			//Printing Firsts
			printf("\nFirst: {");
			for(i=0;i<tempProd->firstCount;i++) printf(" %c",tempProd->first[i]);
			printf(" }\t"); 
			//Printing Follow
			printf("Follow: {");
			for(i=0;i<tempProd->followCount;i++) printf(" %c",tempProd->follow[i]);
			printf(" }\n");
			tempProd = tempProd->prodNext;
		}	
	}
	return 0;
}
