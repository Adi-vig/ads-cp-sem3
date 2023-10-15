#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include<stdlib.h>

#define COL_GREEN  printf("\e[0;32m");
#define COL_RED    printf("\e[0;31m");
#define COL_WHITE  printf("\e[0;37m");
#define COL_YELLOW printf("\e[0;33m");

#define MAX 1000
#define MAXLEN 1000

// random ass line



typedef struct node{
    char ch;
    struct node* child[26];
    bool isWord;

} Node;

char suggestions[MAX][MAXLEN];




//----------------------------------------------------------------
//string stack related
char arrStr[MAX][MAXLEN];
int tos2=-1;
void pushW(char word[]){
    
    strcpy(arrStr[++tos2], word);
    // arrStr[++tos2]=node;
}
char* topW(){
    return arrStr[tos2];
}
char* popW(){
    char* temp=arrStr[tos2--];
    return temp;
}
bool isEmptyW(){
    return (tos2 < 0);
}




//----------------------------------------------------------------
//node stack related
Node* arrNode[MAX];
int tos1=-1;
void pushN(Node* node){
    arrNode[++tos1]=node;
}
Node* topN(){
    return arrNode[tos1];
}
Node* popN(){
    Node* temp=arrNode[tos1--];
    return temp;
}
bool isEmptyN(){
    return (tos1 < 0);
}


int minI(int a, int b){
    return (a < b)?a:b;
}








Node* newNode(){
    Node* temp= (Node*)malloc(sizeof(Node));
    temp->ch = '\0';
    temp->isWord = false;

    for (int i = 0; i < 26; i++) {
        temp->child[i] = NULL;
    }
    return temp;
}




void init(Node* root){
    root=newNode();    
}


Node* insert(Node* root, char word[]){
    Node* currNode = root;
    char currentLetter = '\0';
    int letterIndex = 0;

    int wordLen = strlen(word);
    for (int i = 0; i < wordLen; i++) {
        // convert to lower case
        currentLetter = (char)tolower((int)word[i]);

        // Shift index, ascci table: https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html
        letterIndex = (int)currentLetter - 97;

        // We only accept letters
        if (letterIndex < 0 || letterIndex > 25) break;

        if (currNode->child[letterIndex]){
            // Do nothing
        }
        else {
            // Create new node
            currNode->child[letterIndex] = newNode();
            currNode->child[letterIndex]->ch = currentLetter;
        }
        currNode = currNode->child[letterIndex];
    }
    currNode->isWord = true;

    return root;
}


Node* insertDictionary(Node *root) {
    char DICTIONARY_FILE[] = "dictionary.txt";
    
    FILE *file = fopen ( DICTIONARY_FILE, "r" );

    if (file != NULL) {
        char buff [1000];

        long long count=0;

        while(fgets(buff,sizeof buff,file)!= NULL) {

            if(buff[0]=='#')continue;
            // fprintf(stdout,"%s",buff); //print the file contents on stdout.
            // insert(root, buff);
            root=insert(root, buff);
            count++;
            // if(count == 30) break;

        }
        printf("%ld", count);

        fclose(file);
    }
    else 
        printf("cannot open dictionary.");
    return root;
    
}


bool isLeaf(Node* node) {
    if (node == NULL) return false;

    if (!node->isWord) return false;
    for (int i = 0; i < 26; i++){
        if (node->child[i] != NULL) return false;
    }
    return true;
}








Node* getNode(Node* root,char substring[]) {
    Node* currNode = root;
    char currentLetter = '\0';
    int letterIndex = 0;
    int length = strlen(substring);

    for (int i = 0; i < length; i++){
        currentLetter = substring[i];
        letterIndex = (int)currentLetter - 'a';

        if (letterIndex < 0 || letterIndex > 25) return NULL;

        // If letter does not have matching node
        
        if (currNode->child[letterIndex]==NULL) return NULL;
        else {
            // printf("hell");
            currNode = currNode->child[letterIndex];
        }
    }
    return currNode;
}



char* getCompletions(Node* root, char word[MAXLEN]) {
    char copywod[MAXLEN];
    strcpy(copywod,word);
    Node* parentNode = getNode(root,copywod);
    if (parentNode == NULL || parentNode->ch != word[strlen(word) - 1]) return "";

    Node* currentNode;
    char prefix[MAXLEN];
    // stack<node>tovisit
    pushN(parentNode);

    // each node in toVisit has a corresponding prefix in prefixes


    // Remove last letter from word so that it is not repeated
    // word = word.substr(0, strlen(word) - 1);
    word[strlen(word) - 1] ='\0';
    // std::stack<std::string> prefixes;

    pushW(word);


    // std::vector<std::string> wordsFound;
    int  found=-1;
    char wordsFound[MAX][MAXLEN];

    while (!isEmptyN() ){
        currentNode = topN();
        popN();
        // prefix = prefixes.top();
        strcpy(prefix, topW() );

        // prefixes.pop();
        popW();

        if (isLeaf(currentNode)){
            char strtemp[2];
            strtemp[0]=currentNode->ch;
            strcat(prefix,strtemp );
            if(found < MAX-1)strcpy(wordsFound[++found], prefix );
        }
        else {
            char strtemp[2];
            strtemp[0]=currentNode->ch;
            strcat(prefix,strtemp );
            
                if (currentNode->isWord){
                    if(found < MAX-1)strcpy(wordsFound[++found], prefix );
                }

            // Check all children for completions
            for (int i = 0; i < 26; i++) {
                if (currentNode->child[i] != NULL){
                    pushN(currentNode->child[i]);
                    pushW(prefix);
                }
            }   
        }
    }
    // std::stringstream allWords;
    // for (std::vector<std::string>::iterator it = wordsFound.begin() ; it != wordsFound.end(); ++it){
    //     allWords << *it << "\n";
    // }
    // allWords << "\n";
    // return allWords.str();
    // 
    printf("\n---------Suggestions---------- %d \n",found);
    
    for(int i = 0; i <minI(10,found); i++){
        printf("%d. %s\n",(i+1)%10 ,wordsFound[i]);
        strcpy(suggestions[i],wordsFound[i]);
    }
    return "sd";
}

void tolowercase(char buff[MAXLEN]){
    for(int i = 0; i < MAXLEN; i++){
        if(buff[i]>='A' && buff[i]<'Z'){
            buff[i]= 'a' + buff[i]-'A';
        }

    }
}






int main(){
    
    Node* root;
    root=newNode();
    root=insertDictionary(root);

    // char a='5';
    // char arr[]={4,a,'\0'};
    // printf("%s\n",arr);
    // char buff[MAXLEN];
    // // strcpy(buff,"compli");
    // printf("\nEnter random word to complete\n");
    // scanf("%s",buff);

    // tolowercase(buff);

    // getCompletions(root,buff);

    // printf("\n\nCompleted suggesting \n");

    char str[10000];
    char word[1000];
    int len=0;
    memset(str, '\0',10000); 
    memset(word, '\0',1000); 

    while(1){
        char ch = getch();
        system("cls");
        printf("PRESS Esc to exit ... input : %d %c",ch,ch);

        if(ch == 27)exit(0);
        if(ch == 8 ){
            // getch();
            // if(strlen(str)>0)str[strlen(str)-1] = '\0';

            if(len>0) {word[len-1] ='\0';len--;}
            // printf("Press %s %d",word,strlen(word));
            // strcpy(word,word); 
            
        }

        if(ch >='0' && ch <= '9'){
            memset(word, 0,1000);
            if(ch-'0'-1 >= 0)strcpy(word, suggestions[(ch-'0'-1)%10]);
            else strcpy(word, suggestions[9]);

        }
        
        else{
            // any key else than num back and esc
            char strtemp[2];
            strtemp[0]=ch;
            if(ch==32){
                len=0;
                strcat(str," " );
                strcat(str,word );
                memset(word,'\0',1000); 
            }
            else {strcat(word,strtemp);len++;}
        }


        COL_WHITE
        printf("\n\nType: %s",str);
        printf(" %s",word);
        COL_GREEN
        printf("|");
        COL_RED
        // printf("\n\nCurrword : %s%d",word,strlen(word));
        char copywod[MAXLEN];
        strcpy(copywod,word);
        tolowercase(copywod);
        getCompletions(root,copywod);

    }


    return 0;
}