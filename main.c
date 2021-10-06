#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>

typedef struct node {
	int key;
	int balance;
	struct node *left;
	struct node *right;
}Node;

typedef Node* Tree;

typedef struct elemQ {
	Node *node;
	struct elemQ *next;
}ElemQ;

typedef struct {
	ElemQ *first;
	ElemQ *last;
}Queue;

typedef struct elemS {
	Node *inf;
	int visit;
	struct elemS *next;
}ElemS;

typedef ElemS* Stack;

void SetColor(int color) {
	WORD newColor;
	HANDLE handleStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(handleStdOut, &csbi)) {
		newColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
		SetConsoleTextAttribute(handleStdOut, newColor);
	}
}

char *readLine() {
	char *s = NULL, *ns, c;
	int i = 0;
	while ((c = getchar()) != '\n') {
		if (i % 10 == 0) {
			ns = realloc(s, (i + 10) * sizeof(char));
			if (ns == NULL) {
				printf("Losa alokacija!!!\n");
				free(s);
				exit(0);
			}
			else
				s = ns;
		}
		s[i++] = c;
	}
	ns = realloc(s, (i + 1) * sizeof(char));
	if (ns == NULL) {
		printf("Losa alokacija!!!\n");
		free(s);
		exit(0);
	}
	else
		s = ns;
	s[i] = '\0';
	if (s[0] == '\0') {
		return NULL;
	}
	return s;
}

int** readMatrix(FILE *fin, int *m, int *n) {
	int i, j;
	fscanf(fin, "%d %d", m, n);
	int **mat = malloc((*m) * sizeof(int*));
	if (mat == NULL) {
		printf("Losa alokacija!!!\n");
		exit(1);
	}
	for (i = 0; i < *m; i++) {
		mat[i] = malloc((*n) * sizeof(int));
		if (mat[i] == NULL) {
			printf("Losa alokacija!!!\n");
			exit(2);
		}
		for (j = 0; j < *n; j++) {
			fscanf(fin, "%d", &mat[i][j]);
		}
	}
	return mat;
}

int** generateMatrix(int m, int n, int high) {
	int i, j, low, d = high / m, max = 0;
	srand(time(0));
	int **mat = malloc(m * sizeof(int*));
	if (mat == NULL) {
		printf("Losa alokacija!!!\n");
		exit(1);
	}
	for (i = 0; i < m; i++) {
		mat[i] = malloc(n * sizeof(int));
		if (mat[i] == NULL) {
			printf("Losa alokacija!!!\n");
			exit(2);
		}
		max += d;
		if (i == m - 1)
			max = high;
		for (j = 0; j < n; j++) {
			if (i == 0 && j == 0)
				low = 0;
			else if (i == 0 && j != 0)
				low = mat[i][j - 1];
			else if (i != 0 && j == 0)
				low = mat[i - 1][j];
			else
				low = mat[i - 1][j] > mat[i][j - 1] ? mat[i - 1][j] : mat[i][j - 1];
			mat[i][j] = rand() % (max - low + 1) + low;
		}
	}
	return mat;
}

void searchMatrix(int** mat, int m, int n, int key) {
	int i = 0, j = n - 1;
	while (i < m && j >= 0) {
		if (mat[i][j] == key) {
			printf("Kljuc je pronadjen na poziciji (%d, %d).\n", i, j);
			return;
		}
		else if (mat[i][j] < key)
			i++;
		else
			j--;
	}
	printf("Kljuc nije pronadjen.\n");
}

double evaluateMatrix(int** mat, int m, int n, int *a, int dim) {
	int i, j, k, count, avg = 0, t;
	printf("Evaluacija pretrazivanja matrice:\n");
	printf("\nKLJUC   POZ. KLJUCA   BR. K.\n\n");
	for (k = 0; k < dim; k++) {
		i = 0;
		j = n - 1;
		count = 0;
		t = 0;
		while (i < m && j >= 0) {
			if (mat[i][j] == a[k]) {
				count++;
				printf("%5d   (%4d,%4d)   %6d\n\n", a[k], i, j, count);
				avg += count;
				t = 1;
				break;
			}
			else if (mat[i][j] < a[k])
				i++;
			else
				j--;
			count++;
		}
		if (t == 0) {
			printf("%5d   NEMA KLJUCA   %6d\n\n", a[k], count);
			avg += count;
		}
	}
	printf("Prosek = %lf\n", avg * 1.0 / dim);
	return avg * 1.0 / dim;
}

void printMatrix(int **mat, int m, int n) {
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			printf("%3d ", mat[i][j]);
		}
		printf("\n");
	}
}

void deleteMatrix(int ***mat, int m) {
	int i;
	for (i = 0; i < m; i++) {
		free((*mat)[i]);
	}
	free(*mat);
	*mat = NULL;
}

int* genearteSequence(int dim, int low, int high) {
	int i;
	int *a = malloc(dim * sizeof(int));
	if (a == NULL) {
		printf("Losa alokacija!!!\n");
		exit(1);
	}
	srand(time(0));
	for (i = 0; i < dim; i++) {
		a[i] = rand() % (high - low + 1) + low;
	}
	return a;
}

int* readKeySet(FILE *fin, int *n) {
	int i;
	fscanf(fin, "%d", n);
	int *a = malloc((*n) * sizeof(int));
	if (a == NULL) {
		printf("Losa alokacija!!!\n");
		exit(1);
	}
	for (i = 0; i < *n; i++) {
		fscanf(fin, "%d", &a[i]);
	}
	return a;
}

void deleteArray(int** a) {
	free(*a);
	*a = NULL;
}

Queue createQueue() {
	Queue Q;
	Q.first = Q.last = NULL;
	return Q;
}

int queueEmpty(Queue Q) {
	if (Q.first == NULL)
		return 1;
	return 0;
}

void insert(Queue *Q, Node *n) {
	ElemQ *q = malloc(sizeof(ElemQ));
	if (q == NULL) {
		printf("Losa alokacija!!!\n");
		exit(1);
	}
	q->node = n;
	q->next = NULL;
	if (Q->last == NULL)
		Q->first = q;
	else
		Q->last->next = q;
	Q->last = q;
}

Node* delete(Queue *Q) {
	ElemQ *old;
	Node *n;
	if (Q->first == NULL)
		return NULL;
	n = Q->first->node;
	old = Q->first;
	Q->first = Q->first->next;
	free(old);
	if (Q->first == NULL)
		Q->last = NULL;
	return n;
}

void deleteQueue(Queue *Q) {
	while (Q->first)
		delete(Q);
}

int stackEmpty(Stack S) {
	if (S == NULL)
		return 1;
	return 0;
}

void push(Stack *S, Node *n, int v) {
	ElemS *node = malloc(sizeof(ElemS));
	if (node == NULL) {
		printf("Losa alokacija!!!\n");
		exit(1);
	}
	node->inf = n;
	node->visit = v;
	node->next = *S;
	*S = node;
}

Node* pop(Stack *S) {
	if (stackEmpty(*S))
		return NULL;
	ElemS *el = *S;
	Node *x = (*S)->inf;
	*S = (*S)->next;
	free(el);
	return x;
}

int topV(Stack S) {
	if (!stackEmpty(S))
		return S->visit;
}

Node *newNode(int K) {
	Node *q = malloc(sizeof(Node));
	if (q == NULL) {
		printf("Losa alokacija!!!\n");
		exit(1);
	}
	q->key = K;
	q->balance = 0;
	q->left = q->right = NULL;
	return q;
}

int treeHeight(Tree root) {
	int currw = 1, max = -1, nextw;
	Node *next;
	Queue Q = createQueue();
	insert(&Q, root);
	while (!queueEmpty(Q)) {
		nextw = 0;
		while (currw > 0) {
			currw--;
			next = delete(&Q);
			if (next->left != NULL) {
				insert(&Q, next->left);
				nextw++;
			}
			if (next->right != NULL) {
				insert(&Q, next->right);
				nextw++;
			}
		}
		currw = nextw;
		max++;
	}
	return max;
}

int BSTSearch(Tree root, int K) {
	Node *p = root;
	while (p) {
		if (p->key == K) {
			printf("Kljuc postoji.\n");
			return 1;
		}
		else if (p->key > K)
			p = p->left;
		else
			p = p->right;
	}
	printf("Nema kljuca.\n");
	return 0;
}

void leftRotation(Node **x) {
	Node *y = (*x)->right;
	Node *tmp = y->left;
	y->left = *x;
	(*x)->right = tmp;
}

void rightRotation(Node **x) {
	Node *y = (*x)->left;
	Node *tmp = y->right;
	y->right = *x;
	(*x)->left = tmp;
}

void AVLInsert(Tree *root, int K) {
	Node *fp, *fx, *p, *x, *q, *y;
	int imbal;
	fp = fx = NULL;
	p = x = *root;
	while (p != NULL) {
		if (p->key <= K)
			q = p->right;
		else
			q = p->left;
		if (q != NULL) {
			if (q->balance != 0) {
				fx = p;
				x = q;
			}
		}
		fp = p;
		p = q;
	}
	q = newNode(K);
	if (*root == NULL) {
		*root = q;
		return;
	}
	if (K < fp->key)
		fp->left = q;
	else
		fp->right = q;
	if (K < x->key)
		p = x->left;
	else
		p = x->right;
	y = p;
	while (p != q) {
		if (K < p->key) {
			p->balance = 1;
			p = p->left;
		}
		else {
			p->balance = -1;
			p = p->right;
		}
	}
	if (K < x->key)
		imbal = 1;
	else
		imbal = -1;
	if (x->balance == 0) {
		x->balance = imbal;
		return;
	}
	if (x->balance != imbal) {
		x->balance = 0;
		return;
	}
	if (y->balance == imbal) {
		p = y;
		if (imbal == 1)
			rightRotation(&x);
		else
			leftRotation(&x);
		x->balance = y->balance = 0;
	}
	else {
		if (imbal == 1) {
			p = y->right;
			leftRotation(&y);
			x->left = p;
			rightRotation(&x);
		}
		else {
			p = y->left;
			rightRotation(&y);
			x->right = p;
			leftRotation(&x);
		}
		if (p->balance == 0)
			x->balance = y->balance = 0;
		else {
			if (p->balance == imbal) {
				x->balance = -imbal;
				y->balance = 0;
			}
			else {
				x->balance = 0;
				y->balance = imbal;
			}
			p->balance = 0;
		}
	}
	if (fx == NULL)
		*root = p;
	else {
		if (x == fx->right)
			fx->right = p;
		else
			fx->left = p;
	}
}

Tree formTree(int **mat, int m, int n) {
	Tree root = NULL;
	int i, j;
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			AVLInsert(&root, mat[i][j]);
		}
	}
	return root;
}

void printTree(Tree root) {
	Queue Q = createQueue();
	int i, skip = (int)pow(2, treeHeight(root) + 2) - 2, next;
	insert(&Q, root);
	for (i = 0; i < treeHeight(root) + 1; i++) {
		int j = 1 << i, k, l;
		next = skip;
		skip = (skip - 2) / 2;
		for (k = 0; k <skip; k++) {
			printf(" ");
		}
		for (k = 0; k < j; k++) {
			Node *p = delete(&Q);
			if (p) {
				insert(&Q, p->left);
				insert(&Q, p->right);
			}
			else {
				insert(&Q, NULL);
				insert(&Q, NULL);
			}
			if (p)
				printf("%2d", p->key);
			else
				printf("  ");
			for (l = 0; l < next; l++) {
				printf(" ");
			}
		}
		printf("\n\n");
	}
	deleteQueue(&Q);
}

void printTreeColoured(Tree root) {
	Queue Q1 = createQueue(), Q2 = createQueue();
	int i, skip = (int)pow((double)2, (double)(treeHeight(root) + 1)) + 7 * ((int)pow((double)2, (double)(treeHeight(root))) - 1), next;
	insert(&Q1, root);
	insert(&Q2, root);
	for (i = 0; i < treeHeight(root) + 1; i++) {
		int j = 1 << i, k, l;
		char c = 'l';
		next = skip;
		skip = (skip - 7) / 2;
		for (k = 0; k < j; k++) {
			Node *q = delete(&Q2);
			if (q) {
				insert(&Q2, q->left);
				insert(&Q2, q->right);
			}
			else {
				insert(&Q2, NULL);
				insert(&Q2, NULL);
			}
			if (q) {
				if (q != root) {
					SetColor(3);
					if (c == 'l') {
						printf("   /   ");
					}
					else {
						printf("   \\   ");
					}
					SetColor(7);
				}
			}
			else {
				printf("       ");
			}
			c = (c == 'l') ? 'd' : 'l';
			for (l = 0; l < next; l++) {
				printf(" ");
			}
		}
		printf("\n\n");
		for (k = 0; k < skip + 7; k++) {
			printf(" ");
		}
		for (k = 0; k < j; k++) {
			Node *p = delete(&Q1);
			if (p) {
				insert(&Q1, p->left);
				insert(&Q1, p->right);
			}
			else {
				insert(&Q1, NULL);
				insert(&Q1, NULL);
			}
			if (p) {
				if (p->key < 100) {
					if (p->key < 10)
						printf(" ");
					printf(" ");
				}
				SetColor(1);
				printf("%d", p->key);
				if (p->balance < 0)
					SetColor(4);
				else if (p->balance > 0)
					SetColor(2);
				else
					SetColor(6);
				printf("(%2d)", p->balance);
				SetColor(7);
			}
			else
				printf("       ");
			for (l = 0; l < next; l++) {
				printf(" ");
			}
		}
		printf("\n\n");
		for (k = 0; k < (skip - 7) / 2 + 7; k++) {
			printf(" ");
		}
	}
	deleteQueue(&Q1);
	deleteQueue(&Q2);
}

double evaluateTree(Tree root, int *a, int n) {
	Node *p;
	int i, count, avg = 0, t;
	printf("\nEvaluacija pretrazivanja binarnog stabla:\n");
	printf("\nKLJUC   IMA/NEMA K.   BR. K.\n\n");
	for (i = 0; i < n; i++) {
		p = root;
		count = 0;
		t = 0;
		while (p) {
			if (p->key == a[i]) {
				count++;
				printf("%5d    IMA KLJUCA   %6d\n\n", a[i], count);
				avg += count;
				t = 1;
				break;
			}
			else if (p->key > a[i])
				p = p->left;
			else
				p = p->right;
			count++;
		}
		if (t == 0) {
			printf("%5d   NEMA KLJUCA   %6d\n\n", a[i], count);
			avg += count;
		}
	}
	printf("Prosek = %lf\n\n", avg * 1.0 / n);
	return avg * 1.0 / n;
}

void deleteTree(Tree *root) {
	Node *next = *root;
	Stack S = NULL;
	int v;
	while (next != NULL) {
		push(&S, next, 1);
		next = next->left;
	}
	while (!stackEmpty(S)) {
		v = topV(S);
		next = pop(&S);
		if (v > 0) {
			push(&S, next, -1);
			next = next->right;
			while (next != NULL) {
				push(&S, next, 1);
				next = next->left;
			}
		}
		else {
			free(next);
		}
	}
	*root = NULL;
}


int main() {
	FILE *fin;
	Tree root = NULL, rooteval = NULL;
	int i, j, m, n, **mat = NULL, high, *seq = NULL, *set = NULL, key, dim, min, max, end = 0, input;
	double avgt, avgm;
	char *datin = NULL;
	while (!end) {
		printf("\n1. Unos matrice\n"
			"2. Generisanje matrice\n"
			"3. Ispis matrice\n"
			"4. Pretraga kljuca u matrici\n"
			"5. Evalucija performansi pretrazivanja matrice\n"
			"6. Formiranje binarnog stabla od matrice\n"
			"7. Umetanje kljuca u stablo\n"
			"8. Pretraga kljuca u stablu\n"
			"9. Formatirani ispis stabla\n"
			"10. Evaluacija performansi pretrazivanja matrice i binarnog stabla\n"
			"11. Brisanje matrice i stabla\n"
			"0. Zavrsetak rada\n\n"
			"Vas izbor? ");
		scanf("%d", &input);
		switch (input) {
		case 1: 
			if (mat != NULL)
				deleteMatrix(&mat, m);
			printf("Unesi ime datoteke, odnosno enter za standardni ulaz: ");
			fin = stdin;
			getchar();
			datin = readLine();
			if (datin != NULL) {
				fin = fopen(datin, "r");
				if (fin == NULL) {
					printf("GRESKA\n");
					exit(1);
				}
			}
			if (fin == stdin)
				printf("\nUnesi dimenzije i matricu:\n");
			mat = readMatrix(fin, &m, &n);
			if(fin != stdin)
			    fclose(fin);
			break;
		case 2: 
			if (mat != NULL)
				deleteMatrix(&mat, m);
			printf("Unesi dimenzije matrice i gornju granicu: ");
			scanf("%d %d %d", &m, &n, &high);
			mat = generateMatrix(m, n, high);
			break;
		case 3: 
			if (mat == NULL) {
			    printf("Matrica ne postoji!!!\n");
			    break;
		    }
			printf("Ispis matrice:\n\n");
			printMatrix(mat, m, n);
			break;
		case 4: 
			if (mat == NULL) {
			    printf("Matrica ne postoji!!!\n");
			    break;
		    }
			printf("Unesi kljuc: ");
			scanf("%d", &key);
			searchMatrix(mat, m, n, key);
			break;
		case 5: 
			if (mat == NULL) {
			    printf("Matrica ne postoji!!!\n");
			    break;
		    }
			printf("Unesi dimenziju niza i interval [low, high]: ");
			scanf("%d %d %d", &dim, &min, &max);
			if (seq != NULL)
				deleteArray(&seq);
			seq = genearteSequence(dim, min, max);
			evaluateMatrix(mat, m, n, seq, dim);
			break;
		case 6: 
			if (mat == NULL) {
			    printf("Matrica ne postoji!!!\n");
			    break;
		    }
			if (root != NULL)
			    deleteTree(&root);
			if (rooteval != NULL)
				deleteTree(&rooteval);
			root = formTree(mat, m, n);
			rooteval = formTree(mat, m, n);
			break;
		case 7: 
			if (root == NULL) {
			    printf("Stablo ne postoji!!!\n");
				break;
		    }
			printf("Unesi kljuc koji se umece: ");
			scanf("%d", &key);
			AVLInsert(&root, key);
			break;
		case 8: 
			if (root == NULL) {
			    printf("Stablo ne postoji!!!\n");
			    break;
		    }
			printf("Unesi kljuc koji se pretrazuje: ");
			scanf("%d", &key);
			BSTSearch(root, key);
			break;
		case 9: 
			if (root == NULL) {
			    printf("Stablo ne postoji!!!\n");
			    break;
		    }
			printf("Izgled stabla:\n");
			printTreeColoured(root);
			break;
		case 10: 
			if (mat == NULL) {
			    printf("Matrica ne postoji!!!\n");
				break;
		    }
			if (rooteval == NULL) {
				printf("Stablo ne postoji!!!\n");
				break;
			}
			printf("Unesi ime datoteke, odnosno enter za standardni ulaz: ");
			fin = stdin;
			getchar();
			datin = readLine();
			if (datin != NULL) {
				fin = fopen(datin, "r");
				if (fin == NULL) {
					printf("GRESKA\n");
					exit(0);
				}
			}
			if (fin == stdin)
				printf("\nUnesi dimenziju i kljuceve: ");
			if (set != NULL)
				deleteArray(&set);
			set = readKeySet(fin, &dim);
			if(fin != stdin)
			    fclose(fin);
			avgm = evaluateMatrix(mat, m, n, set, dim);
			avgt = evaluateTree(rooteval, set, dim);
			if (avgm < avgt)
				printf("Pretraga matrice ima bolje performanse.\n");
			else if(avgm > avgt)
				printf("Pretraga binarnog stabla ima bolje performanse.\n");
			else 
				printf("Obe pretrage imaju iste performanse.\n");
			break;
		case 11: 
			if (mat != NULL)
				deleteMatrix(&mat, m);
			if (seq != NULL)
				deleteArray(&seq);
			if (set != NULL)
				deleteArray(&set);
			if (root != NULL)
				deleteTree(&root);
			if (rooteval != NULL)
				deleteTree(&rooteval);
			break;
		case 0: end = 1;
			printf("Kraj.\n");
			break;
		default: printf("Nepostojeci izbor!!!\n");
			break;
		}
	}
	return 0;
}