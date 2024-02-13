/* SAMSON Alexandru-Valentin - 312CC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pixel // pixel
{
    unsigned char r, g, b; // valorile de roșu, verde și albastru ale pixelului
} pixel;

typedef struct node // arborele cuaternar
{
    int r, g, b, size, isLeaf;          // culorile și indicatorul de frunză
    struct node *ult, *urt, *dlt, *drt; // copiii
} treeNode, *pTree;

typedef struct queueCell // celula din coadă
{
    pTree node;             // nodul conținut
    struct queueCell *next; // adresa celulei următoare
} queueCell, *pQueueCell;

typedef struct Queue // coada
{
    pQueueCell front, rear; // adresa feței și a spatelui
} tQueue;

tQueue initQueue() // inițializare coadă
{
    tQueue q;
    q.front = q.rear = NULL;
    return q;
}

int isEmptyQueue(tQueue q) // verificare coadă goală
{
    if (q.front == NULL)
    {
        return 1;
    }

    return 0;
}

tQueue addQueue(tQueue q, pTree node) // adăugare celulă in coadă
{
    pQueueCell p = (pQueueCell)malloc(sizeof(queueCell));

    p->node = node;
    p->next = NULL;
    if (q.front == NULL)
        q.front = q.rear = p;
    else
    {
        q.rear->next = p;
        q.rear = p;
    }

    return q;
}

tQueue delQueue(tQueue q, pTree *node) // scoatere celula din coadă
{
    pQueueCell p;

    *node = q.front->node;
    if (q.front == q.rear)
    {
        free(q.front);
        q.front = q.rear = NULL;
    }
    else
    {
        p = q.front;
        q.front = q.front->next;
        free(p);
    }

    return q;
}

void freeQueue(tQueue q) // eliberare memorie coada
{
    pQueueCell p, aux;

    p = q.front;
    while (p != NULL)
    {
        aux = p;
        p = p->next;
        free(aux);
    }
}

pTree initTree(int red, int green, int blue, int size) // inițializare arbore
{
    pTree t = malloc(sizeof(treeNode));

    t->r = red;
    t->g = green;
    t->b = blue;
    t->size = size;
    t->ult = t->urt = t->dlt = t->drt = NULL;

    return t;
}

void freeTree(pTree t) // eliberare memorie arbore
{
    if (t != NULL)
    {
        freeTree(t->ult);
        freeTree(t->urt);
        freeTree(t->dlt);
        freeTree(t->drt);
        free(t);
    }
}

void freePixels(pixel **p, int height) // eliberare memorie matrice pixeli
{
    int i;

    for (i = 0; i < height; i++)
    {
        free(p[i]);
    }
    free(p);
}

// Calculează valoarea medie de roșu
// a unei anumite zone dreptunghiulare de pixeli
int averageRed(pixel **p, int x, int y, int size)
{
    int i, j, s = size * size;
    unsigned long long red = 0;

    // Parcurge zona dreptunghiulară de pixeli
    for (i = x; i < x + size; i++)
        for (j = y; j < y + size; j++)
        {
            // Adaugă componenta roșie a fiecărui pixel la totalul curent
            red = red + p[i][j].r;
        }

    // Calculează și returnează valoarea medie de
    // roșu împărțind roșul total la numărul de pixeli
    return red / s;
}

// Calculează valoarea medie de verde
// a unei anumite zone dreptunghiulare de pixeli
int averageGreen(pixel **p, int x, int y, int size)
{
    int i, j, s = size * size;
    unsigned long long green = 0;

    // Parcurge zona dreptunghiulară de pixeli
    for (i = x; i < x + size; i++)
        for (j = y; j < y + size; j++)
        {
            // Adaugă componenta verde a fiecărui pixel la totalul curent
            green = green + p[i][j].g;
        }

    // Calculează și returnează valoarea medie de
    // verde împărțind verdele total la numărul de pixeli
    return green / s;
}

// Calculează valoarea medie de albastru
// a unei anumite zone dreptunghiulare de pixeli
int averageBlue(pixel **p, int x, int y, int size)
{
    int i, j, s = size * size;
    unsigned long long blue = 0;

    // Parcurge zona dreptunghiulară de pixeli
    for (i = x; i < x + size; i++)
        for (j = y; j < y + size; j++)
        {
            // Adaugă componenta albastră a fiecărui pixel la totalul curent

            blue = blue + p[i][j].b;
        }

    // Calculează și returnează valoarea medie de
    // albastru împărțind albastrul total la numărul de pixeli
    return blue / s;
}

// Calculează scorul de similitudine
int mean(pixel **p, int x, int y, int size)
{
    int i, j, r, g, b;
    unsigned long long mean = 0;

    // Calculează valorile medii de roșu, verde și albastru pentru zonă
    int red = averageRed(p, x, y, size);
    int green = averageGreen(p, x, y, size);
    int blue = averageBlue(p, x, y, size);

    // Parcurge zona dreptunghiulară de pixeli
    for (i = x; i < x + size; i++)
        for (j = y; j < y + size; j++)
        {
            r = (red - p[i][j].r) * (red - p[i][j].r);
            g = (green - p[i][j].g) * (green - p[i][j].g);
            b = (blue - p[i][j].b) * (blue - p[i][j].b);

            // Adaugă diferențele pătrate la totalul curent
            mean = mean + r + g + b;
        }

    // Calculează și returnează scorul de similitudine
    return mean / (3 * size * size);
}

// Construiește un arbore cuaternar pentru o zonă dreptunghiulară
// dată de pixeli folosind coordonatele x și y și dimensiunea zonei
pTree buildTree(pTree tree, pixel **p, int x, int y, int size, int limit)
{
    // Calculează valorile medii de roșu,
    // verde și albastru pentru pătratul curent
    int red = averageRed(p, x, y, size);
    int green = averageGreen(p, x, y, size);
    int blue = averageBlue(p, x, y, size);

    // Inițializează nodul curent al arborelui
    // cuaternar cu valorile medii RGB și dimensiunea pătratului
    tree = initTree(red, green, blue, size);

    // Dacă valoarea medie RGB a pătratului este mai mare decât
    // limita, împarte pătratul în patru pătrate mai mici și
    // construiește recursiv un arbore cuaternar pentru fiecare pătrat
    if (mean(p, x, y, size) > limit)
    {
        // Pătratul din stânga-sus
        tree->ult = buildTree(tree->ult, p, x, y, size / 2, limit);
        // Pătratul din dreapta-sus
        tree->urt = buildTree(tree->urt, p, x, y + size / 2, size / 2, limit);
        // Pătratul din dreapta-jos
        tree->drt = buildTree(tree->drt, p,
        x + size / 2, y + size / 2, size / 2, limit);
        // Pătratul din stânga-jos
        tree->dlt = buildTree(tree->dlt, p, x + size / 2, y, size / 2, limit);
    }

    // Returnează nodul rădăcină al arborelui cuaternar
    return tree;
}

// Calculează valoarea mai mare dintre două numere
int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }

    return b;
}

// Calculează numărul de nivele din arbor
int levelsNumber(pTree tree)
{
    if (tree == NULL)
    {
        // Arborele este gol, deci nu are nivele
        return 0;
    }
    else
    {
        // Traversează arborele și numără nivelele subarborilor săi
        int ultLevels = levelsNumber(tree->ult);
        int urtLevels = levelsNumber(tree->urt);
        int dltLevels = levelsNumber(tree->dlt);
        int drtLevels = levelsNumber(tree->drt);

        // Numărul de nivele din arbore este numărul maxim de nivele
        // din subarborii săi, plus unul pentru rădăcina arborelui
        int maxSubtreeLevels = max(max(ultLevels, urtLevels),
        max(dltLevels, drtLevels));
        return maxSubtreeLevels + 1;
    }
}

// Calculează câte pătrate întregi au rămas dupa împărțire
int blocksNumber(pTree tree, pixel **p, int x, int y, int size, int limit)
{
    if (tree == NULL)
    {
        // Returnează 0 deoarece nu pot fi formate blocuri din nodul NULL
        return 0;
    }
    else
    {
        // Dacă scorul blocului curent este mai mic sau egal decât limita dată,
        // Atunci se poate forma un singur bloc din acest nod
        if (mean(p, x, y, size) <= limit)
        {
            return 1;
        }
        else
        {
            // Dacă scorul blocului curent este mai mare decât limita dată, 
            // atunci numără recursiv numărul de blocuri din fiecare dintre
            // cele patru cadrane și returnează suma tuturor blocurilor
            return blocksNumber(tree->ult, p, x, y, size / 2, limit) +
            blocksNumber(tree->urt, p, x, y + size / 2, size / 2, limit) +
            blocksNumber(tree->drt, p, x + size / 2,
            y + size / 2, size / 2, limit) +
            blocksNumber(tree->dlt, p, x + size / 2, y, size / 2, limit);
        }
    }
}

// Calculează lungimea laturii celui mai mare pătrat rămas întreg
int biggestSize(pTree tree, int currentSize)
{
    if (tree == NULL)
    {
        // Dacă arborele este gol, nu există noduri de luat în considerare
        return 0;
    }
    else if (tree->ult == NULL)
    {
        // Dacă nodul curent nu are copii,
        // este nedivizat, deci îi returnăm dimensiunea
        return currentSize;
    }
    else
    {
        // Dacă nodul curent are copii, calculăm recursiv
        // dimensiunea celui mai mare nod nedivizat din subarborele acestuia
        int biggestUndividedSize = 0;

        biggestUndividedSize = max(biggestUndividedSize,
        biggestSize(tree->ult, currentSize / 2));
        biggestUndividedSize = max(biggestUndividedSize,
        biggestSize(tree->urt, currentSize / 2));
        biggestUndividedSize = max(biggestUndividedSize,
        biggestSize(tree->dlt, currentSize / 2));
        biggestUndividedSize = max(biggestUndividedSize,
        biggestSize(tree->drt, currentSize / 2));

        return biggestUndividedSize;
    }
}

// Realizează comprimarea imaginii într-un fișier binar
void compress(pTree tree, int size, FILE *f)
{
    unsigned int imageDimension = size;
    unsigned char nodeType, redValue, greenValue, blueValue;
    pTree node = tree;
    tQueue queue = initQueue();

    // Scrierea dimensiunii imaginii în fișier
    fwrite(&imageDimension, sizeof(unsigned int), 1, f);

    // Adăugarea rădăcinii în coadă
    queue = addQueue(queue, node);
    // Parcurgerea cozii
    while (!isEmptyQueue(queue))
    {
        // Extragerea unui nod din coadă
        queue = delQueue(queue, &node);

        // Verificarea dacă nodul are fii
        if (node->ult != NULL)
        {
            // Tipul nodului (0 = intern)
            nodeType = 0;
            fwrite(&nodeType, sizeof(unsigned char), 1, f);

            // Adăugarea fiilor în coadă
            queue = addQueue(queue, node->ult);
            queue = addQueue(queue, node->urt);
            queue = addQueue(queue, node->drt);
            queue = addQueue(queue, node->dlt);
        }
        else
        {
            // Tipul nodului (1 = frunză)
            nodeType = 1;
            fwrite(&nodeType, sizeof(unsigned char), 1, f);

            // Scrierea culorilor RGB în fișier
            redValue = node->r;
            greenValue = node->g;
            blueValue = node->b;
            fwrite(&redValue, sizeof(unsigned char), 1, f);
            fwrite(&greenValue, sizeof(unsigned char), 1, f);
            fwrite(&blueValue, sizeof(unsigned char), 1, f);
        }
    }

    // Eliberarea memoriei alocate pentru coadă
    freeQueue(queue);
}

// Decomprimă imaginea construind
// arborele cuaternar din fișierul binar compresat
unsigned int decompress(pTree node, FILE *f)
{
    unsigned int imageDimension, size;
    unsigned char nodeType, redValue, greenValue, blueValue;

    // Citim dimensiunea imaginii și tipul nodului rădăcină din fișierul binar
    fread(&imageDimension, sizeof(unsigned int), 1, f);
    fread(&nodeType, sizeof(unsigned char), 1, f);

    // Inițializăm dimensiunea imaginii cu valoarea citită
    size = imageDimension;
    // Setăm tipul nodului radacină
    node->isLeaf = nodeType;

    tQueue queue = initQueue();
    // Adăugam nodul rădăcina in coadă
    queue = addQueue(queue, node);

    // Parcurgerea cozii
    while (!isEmptyQueue(queue))
    {
        queue = delQueue(queue, &node);

        // Dacă rădăcina nu este frunză
        if (!node->isLeaf)
        {
            // Actualizăm dimensiunea imaginii
            imageDimension = imageDimension / 2;

            // Citim tipul nodului pentru subarborele din stânga-sus (ult)
            fread(&nodeType, sizeof(unsigned char), 1, f);

            // Inițializăm subarborele din stânga-sus
            node->ult = initTree(0, 0, 0, imageDimension);
            // Setăm tipul nodului
            node->ult->isLeaf = nodeType;

            // Daca nodul nu este frunză, îl adăugăm in coadă
            if (!node->ult->isLeaf)
            {
                queue = addQueue(queue, node->ult);
            }
            // Altfel, citim valorile RGB pentru nodul frunză
            else
            {
                fread(&redValue, sizeof(unsigned char), 1, f);
                fread(&greenValue, sizeof(unsigned char), 1, f);
                fread(&blueValue, sizeof(unsigned char), 1, f);

                node->ult->r = redValue;
                node->ult->g = greenValue;
                node->ult->b = blueValue;
            }

            // Realizăm aceleași operații pentru
            // subarborii din dreapta-sus, dreapta-jos și stânga-jos

            fread(&nodeType, sizeof(unsigned char), 1, f);

            node->urt = initTree(0, 0, 0, imageDimension);
            node->urt->isLeaf = nodeType;
            if (!node->urt->isLeaf)
            {
                queue = addQueue(queue, node->urt);
            }
            else
            {
                fread(&redValue, sizeof(unsigned char), 1, f);
                fread(&greenValue, sizeof(unsigned char), 1, f);
                fread(&blueValue, sizeof(unsigned char), 1, f);

                node->urt->r = redValue;
                node->urt->g = greenValue;
                node->urt->b = blueValue;
            }

            fread(&nodeType, sizeof(unsigned char), 1, f);

            node->drt = initTree(0, 0, 0, imageDimension);
            node->drt->isLeaf = nodeType;
            if (!node->drt->isLeaf)
            {
                queue = addQueue(queue, node->drt);
            }
            else
            {
                fread(&redValue, sizeof(unsigned char), 1, f);
                fread(&greenValue, sizeof(unsigned char), 1, f);
                fread(&blueValue, sizeof(unsigned char), 1, f);

                node->drt->r = redValue;
                node->drt->g = greenValue;
                node->drt->b = blueValue;
            }

            fread(&nodeType, sizeof(unsigned char), 1, f);

            node->dlt = initTree(0, 0, 0, imageDimension);
            node->dlt->isLeaf = nodeType;
            if (!node->dlt->isLeaf)
            {
                queue = addQueue(queue, node->dlt);
            }
            else
            {
                fread(&redValue, sizeof(unsigned char), 1, f);
                fread(&greenValue, sizeof(unsigned char), 1, f);
                fread(&blueValue, sizeof(unsigned char), 1, f);

                node->dlt->r = redValue;
                node->dlt->g = greenValue;
                node->dlt->b = blueValue;
            }
        }
        // Altfel, citim valorile RGB pentru rădăcină
        else
        {
            fread(&redValue, sizeof(unsigned char), 1, f);
            fread(&greenValue, sizeof(unsigned char), 1, f);
            fread(&blueValue, sizeof(unsigned char), 1, f);

            node->r = redValue;
            node->g = greenValue;
            node->b = blueValue;
        }
    }

    // Eliberează coada
    freeQueue(queue);

    // Returnează lungimea laturii imaginii de decompresat
    return size;
}

// Construiește matricea de pixeli după arborele format după decompresie
void buildPixels(pTree node, pixel **p, int x, int y, int size)
{
    int i,j;
    
    if (node == NULL)
    {
        return;
    }

    // Dacă nodul curent este frunză
    if (node->isLeaf)
    {
        // Setează culoarea pixelilor din zona nodului
        for (i = x; i < x + size; i++)
        {
            for (j = y; j < y + size; j++)
            {
                p[i][j].r = node->r;
                p[i][j].g = node->g;
                p[i][j].b = node->b;
            }
        }
    }
    else
    {
        // Traversează recursiv nodurile copil
        buildPixels(node->ult, p, x, y, size / 2);
        buildPixels(node->urt, p, x, y + size / 2, size / 2);
        buildPixels(node->drt, p, x + size / 2, y + size / 2, size / 2);
        buildPixels(node->dlt, p, x + size / 2, y, size / 2);
    }
}

// Crează o imagine cu dimensiunea dată după matricea de pixeli
void createImage(pixel **p, int size, FILE *f)
{
    int i, j;

    // Scrie informațiile de bază ale imaginii în fișier
    fprintf(f, "P6\n%d %d\n255\n", size, size);

    // Parcurge matricea de pixeli și scrie fiecare pixel în fișier
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            fwrite(&p[i][j], sizeof(pixel), 1, f);
        }
    }
}

int main(int argc, char *argv[])
{
    int limit, width, height, maxSize, i, j, size;
    char aux[3], space;
    pTree tree = NULL;
    FILE *fr, *fw;
    pixel **p;

    // Pentru cerința 1
    if (!strcmp(argv[1], "-c1"))
    {
        limit = atoi(argv[2]);
        fr = fopen(argv[3], "rb");
        fw = fopen(argv[4], "w");

        // Citeșe datele imaginii
        fscanf(fr, "%s", aux);
        fscanf(fr, "%d %d\n", &width, &height);
        fscanf(fr, "%d%c", &maxSize, &space);

        // Inițializează și citește matricea de pixeli a imaginii
        p = (pixel **)malloc(width * sizeof(pixel *));
        for (i = 0; i < width; i++)
        {
            p[i] = (pixel *)malloc(height * sizeof(pixel));
        }

        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                fread(&p[i][j], sizeof(pixel), 1, fr);
            }
        }

        // Construiește arborele cuaternar
        tree = buildTree(tree, p, 0, 0, height, limit);
        // Afișează datele corespunzătoare
        fprintf(fw, "%d\n%d\n%d\n", levelsNumber(tree),
        blocksNumber(tree, p, 0, 0, height, limit), biggestSize(tree, height));

        // Eliberează matricea de pixeli
        freePixels(p, height);
    }
    // Pentru cerința 2
    else if (!strcmp(argv[1], "-c2"))
    {
        limit = atoi(argv[2]);
        fr = fopen(argv[3], "rb");
        fw = fopen(argv[4], "wb");

        // Citește datele imaginii
        fscanf(fr, "%s", aux);
        fscanf(fr, "%d %d\n", &width, &height);
        fscanf(fr, "%d%c", &maxSize, &space);

        // Inițializează și citește matricea de pixeli a imaginii
        p = (pixel **)malloc(width * sizeof(pixel *));
        for (i = 0; i < width; i++)
        {
            p[i] = (pixel *)malloc(height * sizeof(pixel));
        }

        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                fread(&p[i][j], sizeof(pixel), 1, fr);
            }
        }

        // Construiește arborele cuaternar
        tree = buildTree(tree, p, 0, 0, height, limit);
        // Realizează comprimarea imaginii într-un fișier binar
        compress(tree, height, fw);

        // Eliberează matricea de pixeli
        freePixels(p, height);
    }
    // Pentru cerința 3
    else if (!strcmp(argv[1], "-d"))
    {
        fr = fopen(argv[2], "rb");
        fw = fopen(argv[3], "wb");

        // Inițializează rădăcina arborelui
        tree = initTree(0, 0, 0, 0);

        // Realizează decomprimarea imaginii
        size = decompress(tree, fr);

        // Inițializează matricea de pixeli a imaginii
        p = (pixel **)malloc(size * sizeof(pixel *));
        for (i = 0; i < size; i++)
        {
            p[i] = (pixel *)malloc(size * sizeof(pixel));
        }

        // Construiește matricea de pixeli
        buildPixels(tree, p, 0, 0, size);
        // Crează imaginea decomprimată
        createImage(p, size, fw);

        // Eliberează matricea de pixeli
        freePixels(p, size);
    }

    // Eliberează arborele cuaternar
    freeTree(tree);

    // Închide fișierele
    fclose(fr);
    fclose(fw);

    return 0;
}
