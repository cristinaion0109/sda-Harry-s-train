/* ION Cristina-Gabriela - 311CC */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// definirea structurii pentru un vagon al trenului
typedef struct TrainNode {
    char info;
    struct TrainNode* next;
    struct TrainNode* prev;
} TrainNode, * TTrainNode;
// definirea structurii pentru tren
typedef struct Train {
    TTrainNode list;
    TTrainNode mechanic;
} Train;
// definirea structurii pentru un nod din coada
typedef struct Cell {
    char elem[20];
    struct Cell* next;
    struct Cell* prev;
} QueueCell, * PQueue;
// definirea structurii pentru coada
typedef struct Queue {
    PQueue front;
    PQueue rear;
} TQueue;
TTrainNode init(TTrainNode sentinel) {
    sentinel = (TTrainNode)malloc(sizeof(TrainNode));
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    return sentinel;
}
// insereaza un vagon la sfarsitul listei
TTrainNode insert(TTrainNode list, char info) {
    TTrainNode new;
    new = (TTrainNode)malloc(sizeof(TrainNode));
    new->info = info;
    TTrainNode last = list->prev;
    new->prev = last;
    new->next = list;
    last->next = new;
    list->prev = new;
    return list;
}
// insereaza un vagon la dreapta mecanicului
Train insert_right(Train train, char info) {
    TTrainNode new;
    new = (TTrainNode)malloc(sizeof(TrainNode));
    new->info = info;
    new->prev = train.mechanic;
    new->next = train.mechanic->next;
    train.mechanic->next->prev = new;
    train.mechanic->next = new;
    train.mechanic = new;
    return train;
}
// insereaza un vagon la stanga mecanicului
Train insert_left(Train train, char info, FILE* out) {
    if (train.mechanic->prev == train.list) {
        fprintf(out, "ERROR\n");
    }
    else {
        TTrainNode new;
        new = (TTrainNode)malloc(sizeof(TrainNode));
        new->info = info;
        new->next = train.mechanic;
        new->prev = train.mechanic->prev;
        train.mechanic->prev->next = new;
        train.mechanic->prev = new;
        train.mechanic = new;
    }
    return train;
}
// muta mecanicul cu o pozitie la stanga
Train move_left(Train train) {
    if (train.mechanic->prev == train.list) {
        // se face mutarea mecanicului la stanga
        train.mechanic = train.list->prev;
    }
    else {
        train.mechanic = train.mechanic->prev;
    }
    return train;
}
// muta mecanicul cu o pozitie la dreapta
Train move_right(Train train) {
    if (train.mechanic->next == train.list) {
        train = insert_right(train, '#');
    }
    else {
        // se face mutarea mecanicului la dreapta
        train.mechanic = train.mechanic->next;
    }
    return train;
}
// afiseaza lista de vagoane
void print(TTrainNode list, FILE* out) {
    TTrainNode i;
    for (i = list->next; i != list; i = i->next) {
        fprintf(out, "%c ", i->info);
    }
    fprintf(out, "\n");
}
// sterge vagonul in care se afla mecanicul
Train del(Train train) {
    TTrainNode pred, succ;
    pred = train.mechanic->prev;
    succ = train.mechanic->next;
    // trenuletul revine la forma initiala
    if (train.mechanic->prev == train.list && train.mechanic->next == train.list) {
        train.mechanic->info = '#';
    }
    else {
        pred->next = succ;
        succ->prev = pred;
        free(train.mechanic);
        // daca mecanicul se afla in primul vagon
        if (train.mechanic->prev == train.list && train.mechanic->next != train.list) {
            // mecanicul este mutat in ultimul vagon
            train.mechanic = train.list->prev;
        }
        else {
            // se muta mecanicul in vagonul din stanga
            train.mechanic = pred;
        }
    }
    return train;
}
// trenul ajunge in starea initiala
Train del_all(Train train) {
    TTrainNode current, aux;
    current = train.list->next->next;
    // parcurgem lista incepand cu al doilea vagon
    while (current != train.list) {
        aux = current->next;
        // stergem fiecare nod, incepand cu al doilea
        free(current);
        current = aux;
    }
    // setam mecanicul in primul vagon
    train.mechanic = train.list->next;
    train.mechanic->next = train.list;
    train.mechanic->info = '#';
    return train;
}
// schimba inscriptionarea vagonului in care se afla mecanicul
Train write(Train train, char c) {
    train.mechanic->info = c;
    return train;
}
// cauta un sir de caractere incepand cu pozitia mecanicului   
Train search(Train train, char s[], FILE* out) {
    TTrainNode current = train.mechanic;
    TTrainNode aux;
    int i = 0;
    int found = 0;
    // cat timp nu am revenit la mecanic
    while (current->next != train.mechanic) {
        if (current == train.list) {
            //sarim peste santinela
            current = current->next;
        }
        //daca caracterul vagonului este egal cu cel de la pozitia curenta din sir
        if (current->info == s[i]) {
            if (i == 0) {
                // memoram pozitia inceputului secventei
                aux = current;
            }
            i++;
            // daca am gasit sirul
            if (i == strlen(s)) {
                // setam mecanicul la nodul unde incepe secventa gasita
                train.mechanic = aux;
                found = 1;
                break;
            }
        }
        else {
            i = 0;
        }
        current = current->next;
    }
    if (current->info == s[strlen(s) - 1] && i == (strlen(s) - 1)) {
        train.mechanic = aux;
        found = 1;
    }
    // daca sirul nu a fost gasit, afisam ERROR
    if (found == 0) {
        fprintf(out, "ERROR\n");
    }
    return train;
}
// cauta un sir de caractere catre dreapta, pornind de la pozitia mecanicului
Train search_right(Train train, char s[], FILE* out) {
    TTrainNode current = train.mechanic;
    // indica daca sirul a fost gasit sau nu
    int i = 0;
    int found = 0;
    while (current != train.list) {
         // daca caracterul curent este egal cu cel din sir la pozitia curenta
        if (current->info == s[i]) {
            // incrementam indexul
            i++;
            // daca am gasit sirul
            if (i == strlen(s)) {
                // setam mecanicul la pozitia curenta
                train.mechanic = current;
                found = 1;
                break;
            }
        }
        else {
            i = 0;
        }
        current = current->next;
    }
    // daca sirul nu a fost gasit, se afiseaza ERROR
    if (found == 0) {
        fprintf(out, "ERROR\n");
    }
    return train;
}
// cauta un sir de caractere catre stanga, pornind de la pozitia mecanicului
Train search_left(Train train, char s[], FILE* out) {
    TTrainNode current = train.mechanic;
    int i = 0;
    // indica daca sirul a fost gasit sau nu
    int found = 0;
    while (current != train.list) {
        // daca caracterul curent este egal cu cel din sir la pozitia curenta
        if (current->info == s[i]) {
            // incrementam indexul
            i++;
            // daca am gasit sirul
            if (i == strlen(s)) {
                // setam mecanicul pe nodul curent
                train.mechanic = current;
                found = 1;
                break;
            }
        }
        else {
            i = 0;
        }
        current = current->prev;
    }
    // daca sirul nu a fost gasit se afiseaza ERROR
    if (found == 0) {
        fprintf(out, "ERROR\n");
    }
    return train;
}
// afiseaza inscriptia vagonului in care se afla mecanicul
void show_current(Train train, FILE* out) {
    fprintf(out, "%c", train.mechanic->info);
    fprintf(out, "\n");
}
// afiseaza continutul trenului
void show(Train train, FILE* out) {
    TTrainNode current = train.list->next;
    //parcurgem trenul nod cu nod
    while (current != train.list) {
        if (current != train.mechanic) {
            fprintf(out, "%c", current->info);
        }
        else {
            fprintf(out, "|%c|", current->info);
        }
        current = current->next;
    }
    fprintf(out, "\n");
}
// initializeaza coada
TQueue init_queue() {
    TQueue q;
    q.front = NULL;
    q.rear = NULL;
    return q;
}
// adauga element in coada
TQueue enqueue(TQueue q, char s[]) {
    PQueue new;
    new = (PQueue)malloc(sizeof(QueueCell));
    strcpy(new->elem, s);
    new->prev = NULL;
    new->next = NULL;
    if (q.front == NULL) {
        q.front = new;
        q.rear = new;
    }
    else {
        q.rear->next = new;
        new->prev = q.rear;
        q.rear = new;
    }
    return q;
}
// se inverseaza ordinea comenzilor din coada
TQueue reverse(TQueue q) {
    PQueue front = q.front;
    PQueue rear = q.rear;
    while (front != rear && front->prev != rear) {
        char aux[20];
        strcpy(aux, front->elem);
        strcpy(front->elem, rear->elem);
        strcpy(rear->elem, aux);
        front = front->next;
        rear = rear->prev;
    }
    return q;
}
// se executa comenzile din coada
void execute_queue(TQueue* q, Train* train, FILE* out) {
    if ((*q).front == NULL && (*q).rear == NULL) {
        fprintf(out, "queue is empty\n");
        return;
    }
    else {
        PQueue aux = (*q).front;
        if (strcmp(aux->elem, "MOVE_LEFT") == 0) {
            *train = move_left(*train);
        }
        else if (strcmp(aux->elem, "MOVE_RIGHT") == 0) {
            *train = move_right(*train);
        }
        else if (strstr(aux->elem, "WRITE") != NULL) {
            char* p;
            p = aux->elem;
            p = strtok(p, " ");
            p = strtok(NULL, " ");
            *train = write(*train, p[0]);
        }
        else if (strcmp(aux->elem, "CLEAR_CELL") == 0) {
            *train = del(*train);
        }
        else if (strcmp(aux->elem, "CLEAR_ALL") == 0) {
            *train = del_all(*train);
        }
        else if (strstr(aux->elem, "INSERT_RIGHT") != NULL) {
            char* p;
            p = aux->elem;
            p = strtok(p, " ");
            // pointeaza catre caracterul de care avem nevoie
            p = strtok(NULL, " ");
            *train = insert_right(*train, p[0]);
        }
        else if (strstr(aux->elem, "INSERT_LEFT") != NULL) {
            char* p;
            p = aux->elem;
            p = strtok(p, " ");
            p = strtok(NULL, " ");
            *train = insert_left(*train, p[0], out);
        }
        else if (strstr(aux->elem, "SEARCH") != 0 && strstr(aux->elem, "SEARCH_LEFT") == 0 && strstr(aux->elem, "SEARCH_RIGHT") == 0) {
            char* p;
            p = aux->elem;
            p = strtok(p, " ");
            p = strtok(NULL, " ");
            *train = search(*train, p, out);
        }
        else if (strstr(aux->elem, "SEARCH_LEFT") != 0) {
            char* p;
            p = aux->elem;
            p = strtok(p, " ");
            p = strtok(NULL, " ");
            *train = search_left(*train, p, out);
        }
        else if (strstr(aux->elem, "SEARCH_RIGHT") != 0) {
            char* p;
            p = aux->elem;
            p = strtok(p, " ");
            p = strtok(NULL, " ");
            *train = search_right(*train, p, out);
        }
        (*q).front = aux->next;
        free(aux);
    }
}
// se citesc comenzile si se executa
void commands(TQueue* q, Train train, int n, FILE* in, FILE* out) {
    int i;
    char s[30];
    for (i = 0; i < n; i++) {
        fgets(s, 30, in);
        // adaugam terminatorul de siruri
        s[strlen(s) - 1] = '\0';
        if (strcmp(s, "MOVE_LEFT") == 0) {
            *q = enqueue(*q, s);
        }
        else if (strcmp(s, "MOVE_RIGHT") == 0) {
            *q = enqueue(*q, s);
        }
        else if (strstr(s, "WRITE") != NULL) {
            *q = enqueue(*q, s);
        }
        else if (strcmp(s, "CLEAR_CELL") == 0) {
            *q = enqueue(*q, s);
        }
        else if (strcmp(s, "CLEAR_ALL") == 0) {
            *q = enqueue(*q, s);
        }
        else if (strstr(s, "INSERT_RIGHT") != NULL) {
            *q = enqueue(*q, s);
        }
        else if (strstr(s, "INSERT_LEFT") != NULL) {
            *q = enqueue(*q, s);
        }
        else if (strstr(s, "SEARCH") != 0 && strstr(s, "SEARCH_LEFT") == 0 && strstr(s, "SEARCH_RIGHT") == 0) {
            *q = enqueue(*q, s);
        }
        else if (strstr(s, "SEARCH_LEFT") != 0) {
            *q = enqueue(*q, s);
        }
        else if (strstr(s, "SEARCH_RIGHT") != 0) {
            *q = enqueue(*q, s);
        }
        else if (strcmp(s, "SHOW_CURRENT") == 0) {
            show_current(train, out);
        }
        else if (strcmp(s, "SHOW") == 0) {
            show(train, out);
        }
        else if (strcmp(s, "SWITCH") == 0) {
            *q = reverse(*q);
        }
        else if (strcmp(s, "EXECUTE") == 0) {
            execute_queue(q, &train, out);
        }
    }
}
int main() {
    // se deschid fisierele
    FILE* in = fopen("tema1.in", "r");
    FILE* out = fopen("tema1.out", "w");
    if (in == NULL || out == NULL) {
        printf("OPENING ERROR");
        return -1;
    }
    Train train;
    train.list = NULL;
    train.list = init(train.list);
    train.list = insert(train.list, '#');
    // setam mecanicul in primul vagon
    train.mechanic = train.list->next;
    TQueue q;
    q = init_queue(q);
    int n;
    char c;
    // se citeste numarul de comenzi din fisier
    fscanf(in, "%d", &n);
    fscanf(in, "%c", &c);
    commands(&q, train, n, in, out);
    TTrainNode current = train.list->next;
    // eliberam memoria pentru fiecare nod al trenului
    while (current != train.list) {
        TTrainNode aux1 = current;
        current = current->next;
        free(aux1);
    }
    free(train.list);
    PQueue current_queue = q.front;
    // eliberam memoria pentru fiecare nod al cozii
    while (current_queue != NULL) {
        PQueue aux2 = current_queue;
        current_queue = current_queue->next;
        free(aux2);
    }
    // inchidem fisierele
    fclose(in);
    fclose(out);
    return 0;
}
