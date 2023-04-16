#include <stdio.h>
#include <stdlib.h>
#define DIM_ALFABETO 64
#define MAX_LUNG_CMD 17
#define FINE_PROGRAMMA 0
#define NUOVA_PARTITA 1
#define INSERISCI_INIZIO 2
#define INSERISCI_FINE 3
#define STAMPA_FILTRATE 4
#define PAROLA_LETTA 5


typedef struct nodo_{
    char ok;
    struct nodo_* filt;
    struct nodo_* left;
    struct nodo_* right;
    char word[];
}RB_node;

RB_node* root=NULL;
RB_node* root_filtrate=NULL;
RB_node* filt_temp;
int dim_str=0, go=0, game_on=0, n=0, insert=0, equal=1, conta=0;
RB_node* tmp, *prec;
char* next_cmd;
char* riferimento;
char* res;
short int** vincoli;
int* c;
int hash_vet[78], inv_vet[64];

//funzioni della struttura dati
int hash_function(char);
char inverse_hash_function(int);
RB_node* INSERT();
int TREE_SEARCH(RB_node*, char*, int);
void INORDER_TREE_WALK(RB_node*);
void  RESET_FILTRATE(RB_node*);
int lexicographic_order(char* , char*, int);
void DELETE_TREE(RB_node*);

//funzioni per confrontare
void Swap(int*, int*);
void BuildMaxHeap(int [], int, int[]);
void HeapSort(int* , int, int []);
void conta_lettere(char*, int[], int);
void conta_corrette(char*, char*, int[], int);
void compare(char*, char*, char*, int);

//funzioni per contare le parole ammissibili e stamparle
int max(int, int);
void number_symbol_merge(char*, char*, int[], int);
void costruzione_vincoli(short int**, char*, char*, int);
int check_vincoli(short int**, char*, int);
int conta_correnze(char, char*, int);
void stampa_filtrate(int*, short int**);

int main(){
    int x;

    c=&conta;

    hash_vet[0]=0;
    for (int i=48-45;i<48-45+10;i++) hash_vet[i]=i-2;
    for (int i=65-45;i<65-45+26;i++) hash_vet[i]=i-9;
    for (int i=97-45;i<97-45+26;i++) hash_vet[i]=i-14;
    hash_vet[95-45]=37;

    inv_vet[0]=45;
    for (int i=1;i<11;i++) inv_vet[i]=i+48-1;
    for (int i=11;i<11+26;i++) inv_vet[i]=i+65-11;
    inv_vet[37]=95;
    for (int i=38;i<38+26;i++) inv_vet[i]=i+97-38;


    go=scanf("%d",&dim_str); //leggo lunghezza parole

    root_filtrate=malloc(sizeof(RB_node)+ (dim_str+1)*sizeof(char));

    //alloco dinamicamente le stringhe in cui salvare l'input
    if(MAX_LUNG_CMD>=dim_str)
        next_cmd = malloc(sizeof(char) * (MAX_LUNG_CMD + 1));
    else
        next_cmd=malloc(sizeof(char)*(dim_str+1));
    riferimento=malloc(sizeof(char)*(dim_str+1));
    res=malloc(sizeof(char)*(dim_str+1));

    //inizializzo matrice dinamica per vincoli parole ammissibili
    vincoli= malloc(sizeof (int*)*(dim_str+2));
    for(int i=0; i<(dim_str+2); i++) {
        *(vincoli + i) = malloc(sizeof(int) * DIM_ALFABETO);
        for (int j = 0; j < DIM_ALFABETO; j++)
            *(*(vincoli + i) + j) = -1;
    }

    //inizia il programma
    while(go!=0){
        //decide
        x=scanf("%s", next_cmd);
        if(x==EOF)
            go=FINE_PROGRAMMA;
        else if(next_cmd[0]==' ')
            go=-1;
        else if(next_cmd[0]!='+')
            go=PAROLA_LETTA;
        else if(next_cmd[0]=='+' && next_cmd[1]=='n')
            go=NUOVA_PARTITA;
        else if(next_cmd[0]=='+' && next_cmd[1]=='s')
            go=STAMPA_FILTRATE;
        else if(next_cmd[0]=='+' && next_cmd[1]=='i' && next_cmd[11]=='i')
            go=INSERISCI_INIZIO;
        else if(next_cmd[0]=='+' && next_cmd[1]=='i' && next_cmd[11]=='f')
            go=INSERISCI_FINE;

        if (go == NUOVA_PARTITA) {
            game_on = 1;
            *c=0;
            go = scanf("%s", riferimento);
            go = scanf("%d", &n);
            for(int i=0; i<(dim_str+2); i++)
                for (int j = 0; j < DIM_ALFABETO; j++)
                    *(*(vincoli + i) + j) = -1;

            filt_temp=root_filtrate->filt;
            while(filt_temp){
                filt_temp->ok='1';
                filt_temp=filt_temp->filt;
            }
        }
        if (go == INSERISCI_INIZIO && insert == 0) {
            insert = 1;
        }
        if (go == INSERISCI_FINE && insert == 1)
            insert = 0;
        if(go==STAMPA_FILTRATE) {
            *c=0;
            stampa_filtrate(c, vincoli);
            INORDER_TREE_WALK(root);
        }
        if (go == PAROLA_LETTA) {
            if (game_on == 1) {
                if (insert == 1) {

                    tmp = malloc(sizeof(RB_node) + (dim_str + 1) * sizeof(char));
                    tmp->ok='1';
                    tmp->filt = NULL;
                    tmp->right = NULL;
                    tmp->left = NULL;
                    for (int i = 0; i < dim_str; i++)
                        tmp->word[i] = next_cmd[i];

                    prec=root_filtrate;
                    tmp->filt=root_filtrate->filt;
                    root_filtrate->filt=tmp;

                    root = INSERT();

                }
                else if (TREE_SEARCH(root, next_cmd, dim_str) == 1) {

                    compare(next_cmd, riferimento, res, dim_str);
                    costruzione_vincoli(vincoli, next_cmd, res, dim_str);
                    *c=0;
                    stampa_filtrate(c, vincoli);

                    for (int i = 0; i < dim_str && equal == 1; i++) {
                        if (res[i] != '+')
                            equal = 0;
                    }
                    if (equal == 1) {
                        printf("ok\n");
                        game_on = 0;
                        filt_temp=root_filtrate;
                        RESET_FILTRATE(root);
                    } else{
                        printf("%s\n", res);
                        printf("%d\n", conta);
                        n--;
                        equal=1;
                    }
                    if (n == 0) {
                        printf("ko\n");
                        game_on = 0;
                        filt_temp=root_filtrate;
                        RESET_FILTRATE(root);
                    }
                } else
                    printf("not_exists\n");
            } else {
                tmp = malloc(sizeof(RB_node) + (dim_str + 1) * sizeof(char));
                tmp->ok='1';
                tmp->filt = NULL;
                tmp->right = NULL;
                tmp->left = NULL;
                for (int i = 0; i < dim_str; i++)
                    tmp->word[i] = next_cmd[i];

                prec=root_filtrate;
                tmp->filt=root_filtrate->filt;
                root_filtrate->filt=tmp;

                root = INSERT();
            }
        }
    }

    return 0;
}

int hash_function(char lettera){
    return hash_vet[lettera-45];
}

char inverse_hash_function(int pos){
    return inv_vet[pos];

}

RB_node* INSERT(){
    RB_node * prec=NULL, *curr=root;

    if(curr==NULL)
        return tmp;

    while(curr!=NULL){
        prec=curr;
        if(lexicographic_order(tmp->word, curr->word, dim_str)==1)
            curr=curr->left;
        else
            curr=curr->right;
    }
    if(lexicographic_order(tmp->word, prec->word, dim_str)==1)
        prec->left=tmp;
    else
        prec->right=tmp;

    return root;
}

void INORDER_TREE_WALK(RB_node* root){
    if(root!=NULL){
        INORDER_TREE_WALK(root->left);
        if(root->ok=='1')
            printf("%s\n", root->word);
        INORDER_TREE_WALK(root->right);
    }
}

void RESET_FILTRATE(RB_node* root){
    if(root!=NULL){

        RESET_FILTRATE(root->left);
        RESET_FILTRATE(root->right);
        filt_temp->filt=root;
        filt_temp=root;
    }
}

int TREE_SEARCH(RB_node* root, char* str, int dim_str){
    int flag=1;
    RB_node * curr=root;

    while(curr!=NULL) {

        flag= lexicographic_order(str, curr->word, dim_str);

        if (flag==0)
            return 1;
        else if (flag==1)
            curr = curr->left;
        else
            curr = curr->right;
        flag=1;
    }
    return 0;
}

int lexicographic_order(char* str1, char* str2, int dim_str){
    for(int i=0; i<dim_str; i++){
        if(str1[i]!=str2[i]) {
            if (str1[i] < str2[i])
                return 1;
            else
                return 2;
        }
    }
    return 0;
}

void DELETE_TREE(RB_node* root){
    if (root != NULL) {

        DELETE_TREE(root->left);
        DELETE_TREE(root->right);

        free(root);
    }
}

void Swap(int *a, int *b){
    int tmp=*a;
    *a=*b;
    *b=tmp;
}

void BuildMaxHeap(int arr[], int lenght, int index[]){
    int pos;

    for(int i=1; i<lenght; i++){
        if(arr[i]>arr[(i-1)/2]){
            pos=i;
            while(arr[pos]>arr[(pos-1)/2]){
                Swap(&arr[pos], &arr[(pos-1)/2]);
                Swap(&index[pos], &index[(pos-1)/2]);
                pos=(pos-1)/2;
            }
        }
    }
}

void HeapSort(int array[], int n, int index[]){
    int j, z;

    for(int i=0; i<n; i++)
        index[i]=i;

    BuildMaxHeap(array, n, index);
    for(int i=n-1; i>0; i--){
        Swap(&array[0], &array[i]);
        Swap(&index[0], &index[i]);
        j=0;
        do {
            z=2*j+1;
            if(array[z]<array[z+1] && z<(i-1))
                z++;
            if(array[j]<array[z] && z<i) {
                Swap(&array[j], &array[z]);
                Swap(&index[j], &index[z]);
            }
            j=z;
        }while(z<i);
    }
}

void conta_lettere(char* stringa,int vettore[], int dim_str){
    for(int i=0; i<dim_str; i++){
        vettore[hash_function(stringa[i])]++;
    }
}

void conta_corrette(char* p, char* r, int corrette[], int dim_str){
    for(int i=0; i<dim_str; i++)
        if(p[i]==r[i])
            corrette[hash_function(p[i])]++;
}

void compare(char p[], char r[], char res[],  int dim_str){
    int istanze[DIM_ALFABETO];
    int corrette[DIM_ALFABETO];
    int sbagliate[DIM_ALFABETO];
    int pos;
    for(int i=0; i<DIM_ALFABETO; i++) {
        corrette[i]=0;
        istanze[i]=0;
        sbagliate[i]=0;
    }
    conta_lettere(r, istanze, dim_str);
    conta_corrette(p, r, corrette, dim_str);
    for(int i=0; i<dim_str; i++){
        pos= hash_function(p[i]);

        if(p[i]==r[i])
            res[i] = '+';
        else{
            sbagliate[pos]++;

            if(istanze[pos]==0)
                res[i]='/';

            else {
                if ((istanze[pos] - corrette[pos]) < sbagliate[pos])
                    res[i] = '/';
                else
                    res[i] = '|';
            }
        }
    }
}

int max(int a, int b){
    if(a>=b)
        return a;
    else
        return b;
}

void number_symbol_merge(char* p, char* res, int merge[], int dim_str){
    for(int i=0; i<dim_str; i++){
        merge[i]= hash_function(p[i])*3;
        if(res[i]=='|')
            merge[i]++;
        else if(res[i]=='/')
            merge[i]=merge[i]+2;
    }
}

void costruzione_vincoli(short int** vincoli, char* p, char* res, int dim_str){
    int merge[dim_str];
    int index[dim_str];
    int tot_car=0, min_car=0;
    int symbol, letter, prec, tot_=0;

    number_symbol_merge(p, res, merge, dim_str);
    HeapSort(merge, dim_str, index);

    prec=-1;
    for(int i=0; i<dim_str; i++){
        letter=merge[i]/3;
        symbol=merge[i]%3;

        if(letter!=prec) {
            if(vincoli[dim_str][prec]==vincoli[dim_str+1][prec]) {
                vincoli[dim_str][prec] = max(vincoli[dim_str][prec], tot_car);

                if (vincoli[dim_str][prec] >= 0)
                    vincoli[dim_str + 1][prec] = max(vincoli[dim_str + 1][prec], min_car);
            }
            if(tot_car>min_car)
                vincoli[dim_str+1][prec]=0;
            tot_car = 0;
            min_car = 0;
            tot_=0;
        }
        if(symbol==0){
            for(int j=0; j<DIM_ALFABETO; j++) {
                vincoli[index[i]][j]=0;
            }
            vincoli[index[i]][letter]=1;
            tot_car++;
            min_car++;
        }
        else if(symbol==1){
            vincoli[index[i]][letter]=0;
            tot_++;
            tot_car++;
            min_car++;
        }
        else if(symbol==2){
            if(tot_car>0) {
                if(tot_>0){
                    vincoli[index[i]][letter] = 0;
                }
                else{
                    for(int z=0; z<dim_str; z++){
                        if(vincoli[z][letter]!=1)
                            vincoli[z][letter]=0;
                    }
                }
                min_car = 0;
            }
            else
                for(int j=0; j<dim_str; j++)
                    vincoli[j][letter]=0;

        }
        prec=letter;
        if(i==dim_str-1){
            if(vincoli[dim_str][prec]==vincoli[dim_str+1][prec]) {
                vincoli[dim_str][prec] = max(vincoli[dim_str][prec], tot_car);
                if (vincoli[dim_str][prec] >= 0)
                    vincoli[dim_str + 1][prec] = max(vincoli[dim_str + 1][prec], min_car);
            }
            if(tot_car>min_car)
                vincoli[dim_str+1][prec]=0;
        }
    }
}

int check_vincoli(short int** vincoli, char* str, int dim_str){
    int c;
    char h;
    for (int i = 0; i < dim_str; i++){
        if (vincoli[i][hash_function(str[i])] == 0) return 0;
    }
    for(int j=0; j<DIM_ALFABETO; j++){
        if (vincoli[dim_str][j]>0) {
            h= inverse_hash_function(j);
            c= conta_correnze(h, str, dim_str);
            if((vincoli[dim_str][j]>vincoli[dim_str+1][j]) && (c != vincoli[dim_str][j]))
                return 0;
            if((vincoli[dim_str][j]==vincoli[dim_str+1][j]) && (c < vincoli[dim_str][j]))
                return 0;
        }
    }
    return 1;
}

int conta_correnze(char let, char* str, int dim_str){
    int correnze=0;
    for(int i=0; i<dim_str; i++){
        if(let==str[i])
            correnze++;
    }
    return correnze;
}

void stampa_filtrate(int* conta, short int** vincoli){

    RB_node* curr=root_filtrate->filt;
    RB_node* prec=root_filtrate;

    while(curr!=NULL) {

        if (check_vincoli(vincoli, curr->word, dim_str) == 1) {
            *conta = *conta + 1;
            prec=prec->filt;
            curr=curr->filt;
        } else{
            curr->ok = '0';
            prec->filt = curr->filt;
            curr=prec->filt;
        }
    }
}

