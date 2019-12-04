#include <stdio.h>
#include <assert.h>
#include <string.h>

#define _SUM(left, right) CreateNode (SUM, "+", left, right)
#define _SUB(left, right) CreateNode (SUB, "-", left, right)
#define _MUL(left, right) CreateNode (MUL, "*", left, right)
#define _DIV(left, right) CreateNode (DIV, "/", left, right)
#define _VAR CreateNode (1)
#define _NUM CreateNode (0)
#define cL CopyNode (node->left)
#define cR CopyNode (node->right)
#define dL DifNode (node->left)
#define dR DifNode (node->right)
#define _L root->left
#define _R root->right


enum {
    NUM,
    SUM,
    SUB,
    MUL,
    DIV,
    VAR
};
const char* s = "";
const int DataSize = 100;
const int Question = 1;
const int Object = 2;

struct Node
{
    char* data;
    double num;
    Node* left;
    Node* right;
    int type;
};


Node* GetG (const char* str);
Node* GetE ();
Node* GetT ();
Node* GetP ();
Node* GetF ();
Node* GetN ();



Node* CreateNode (int type, char* data, Node* left, Node* right);
Node* CreateNode (double num);
Node* CopyNode (Node* node);
Node* DifNode (const Node* node);
Node* DelNode (Node* node);
void DeleteTree (Node* root);
Node* GetTreeFromFile (Node* root, FILE* f_in);
void PrintNods (Node* node, FILE* f_out);
void NodePrint (Node* node, FILE* f_out);
void TreePrint (Node* root, FILE* f_out);
void SaveTree (Node* node, int RootType, FILE* f_sav);
void TreeToLaTex (Node* root, Node* d_root, FILE* f_tex);
void Simplification (Node* root);
void CopyTo (Node* root, Node* NewNode);

Node* operator+ (Node a, Node b) {
    return CreateNode (SUM, "+", (&(a)), (&(b)));
}

int main () {
    FILE* f_out = fopen ("F:\\Graphs\\output.dot", "w");
    Node* root = GetG ("x*x*x-17+3*(x+9*x*x)");
    Node* d_root = DifNode (root);
    Simplification (d_root);
    TreePrint (d_root, f_out);

    FILE* f_tex = fopen ("F:\\LaTex\\output.tex", "w");
    assert (f_tex);

    TreeToLaTex(root, d_root, f_tex);
    return 0;
}

Node* GetG (const char* str) {
    s = str;
    Node* val = GetE ();
    assert (*s == '\0');
    return val;
}

Node* GetE () {
    Node* val = nullptr;
    val = GetT ();
    while (*s == '+' || *s == '-') {
        char op = *s;
        s++;
        Node* val2 = GetT ();
        if (op == '+') {
            val = _SUM (val, val2);
        }
        else
            val = _SUB(val, val2);
    }
    return val;
}

Node* GetT () {
    Node* val = nullptr;
    val = GetP ();
    while (*s == '*' || *s == '/') {
        char op = *s;
        s++;
        Node* val2 = GetP ();
        if (op == '*') {
            val = _MUL (val, val2);
            //val *= val2;
        }
        else
            val = _DIV (val, val2);
            //val /= val2;
    }
    return val;
}

Node* GetP () {
    Node* val = nullptr;
    if (*s == '(') {
        s++;
        val = GetE();
        assert (*s == ')');
        s++;
        return val;
    }
    else if (*s == 'x') {
        s++;
        return CreateNode (VAR, "x", nullptr, nullptr);
    }
    else if ('0' <= *s && *s <= '9')
        return GetN ();
    else
        return GetF ();
}

Node* GetF () {
    Node* val = nullptr;
//functions

    return val;
}

Node* GetN () {
    Node* res = nullptr;
    double val = 0.0;
    int n = 0;

    sscanf (s, "%lf%n", &val, &n);
    s += n;
    res = CreateNode (val);
    return res;
}

Node* CreateNode (int type, char* data, Node* left, Node* right) {

    Node* node  = (Node*) calloc (1, sizeof (Node));
    node->data  = (char*) calloc (1, sizeof (double));
    strcpy (node->data, data);
    node->left  = left;
    node->right = right;
    node->type  = type;

    return node;
}

Node* CreateNode (double num) {
    Node* node  = (Node*) calloc (1, sizeof (Node));
    node->num  = num;
    node->left  = nullptr;
    node->right = nullptr;
    node->type  = NUM;

    return node;
}

void Simplification (Node* root) {

    if (_L && _R) {
        Simplification(_L);
        Simplification(_R);
        Node* NewNode = nullptr;
        if (_L->type == NUM && _R->type == NUM) {
            switch (root->type) {
                case SUM:
                    NewNode = CreateNode(_L->num + _R->num);
                    CopyTo (root, NewNode);
                    break;
                case SUB:
                    NewNode = CreateNode (_L->num - _R->num);
                    CopyTo (root, NewNode);
                    break;
                case MUL:
                    NewNode = CreateNode (_L->num * _R->num);
                    CopyTo (root, NewNode);
                    break;
            }
        }

        if (root->type == MUL) {
            if ((_L->type == NUM && _L->num == 0) || (_R->type == NUM && _R->num == 0)) {
                NewNode = CreateNode (0);
                CopyTo (root, NewNode);
            }
            else if (_L->type == NUM && _L->num == 1) {
                CopyTo (root, _R);
            }
            else if (_R->type == NUM && _R->num == 1) {
                CopyTo (root, _L);
            }
        }

        if (root->type == SUM) {
            if (_L->type == NUM && _L->num == 0) {
                CopyTo (root, _R);
            }
            else if (_R->type == NUM && _R->num == 0) {
                CopyTo (root, _L);
            }
        }

        free (NewNode);
    }
}

void CopyTo (Node* root, Node* NewNode) {
    root->left = NewNode->left;
    root->right = NewNode->right;
    root->num = NewNode->num;
    root->type = NewNode->type;
    root->data = NewNode->data;
}

Node* DifNode (const Node* node) {
        switch (node->type) {
            case NUM:
                return _NUM;
            case VAR:
                return _VAR;
            case SUM:
                return _SUM (dL, dR);
            case SUB:
                return _SUB (dL, dR);
            case MUL:
                return _SUM (_MUL(dL, cR), _MUL(cL, dR));

        }
}

Node* CopyNode (Node* node) {

    switch (node->type) {
        case NUM:
            return CreateNode (node->num);
        case VAR:
            return CreateNode (VAR, "x", nullptr, nullptr);
        case MUL:
            return _MUL(node->left, node->right);
        case DIV:
            return _DIV(node->left, node->right);
        case SUM:
            return _SUM(node->left, node->right);
        case SUB:
            return _SUB(node->left, node->right);
        default:
            return nullptr;
    }
}

Node* DelNode (Node* node) {
    free (node->data);
    free (node->left);
    free (node->right);
}

Node* GetTreeFromFile (Node* root, FILE* f_in) {

    fscanf (f_in, "(");
    char* data = (char*) calloc (DataSize, sizeof (char));
    int n = 0;
    fscanf (f_in, "%[^()] %n", data, &n);
    if (n == 0)
        return nullptr;
    int type = Object;
    if (*data == '?') type = Question;
    if (type == Question) data = data + 1;
    //root = CreateNode (type, data);
    root->left  = GetTreeFromFile (root->left, f_in);
    root->right = GetTreeFromFile (root->left, f_in);
    fscanf (f_in,")");
    return root;
}

void DeleteTree (Node* root) {

    if (root) {
        DeleteTree (root->left);
        DeleteTree (root->right);
        DelNode (root);
    }
}

void TreeToLaTex (Node* root, Node* d_root, FILE* f_tex) {
    fprintf (f_tex, "\\documentclass[a4paper,12pt]{article}\n"
                    "\n"
                    "\\usepackage[T2A]{fontenc}\n"
                    "\\usepackage[utf8]{inputenc}\n"
                    "\\usepackage[english,russian]{babel}\n"
                    "\n"
                    "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools} \n"
                    "\n"
                    "\n"
                    "\\usepackage{wasysym}\n"
                    "\n"
                    "\\author{Беляев Анастасиус}\n"
                    "\\title{Небольшой пример взятия производной}\n"
                    "\\date{\\today}\n"
                    "\n"
                    "\n"
                    "\\begin{document}\n"
                    "\n"
                    "\\maketitle\n"
                    "\\newpage\n"
                    "\n"
                    "Наша первая строчка.\\\\[1cm]\n"
                    "\n"
                    "\\subsection{Дроби}\n"
                    "\n"
                    "{\\Large Красиво так}:\n"
                    "\n"
                    "\\[ \\frac{1 + 3 \\cdot 5}{3}+\\frac{1}{3}=\\frac{2}{3} \\]\\\\[2cm]\n"
                    "\n"
                    "{\\Large Производная от} $");
    SaveTree(root, root->type, f_tex);
    fprintf (f_tex, "$:\n"
                    "\\[");
    SaveTree(d_root, d_root->type, f_tex);
    fprintf (f_tex,"\\]\n"
                   "\\end{document}");

    fclose (f_tex);
}

void SaveTree (Node* node, int RootType, FILE* f_sav) {
    if (node) {
        if ((node->type == SUM || node->type == SUB) && RootType == MUL)
            fprintf(f_sav, "(");
        SaveTree(node->left, node->type, f_sav);

        if (node->type == NUM)
            fprintf(f_sav, "%.2lf", node->num);
        else
            fprintf(f_sav, "%s", node->data);

        SaveTree(node->right, node->type, f_sav);
        if ((node->type == SUM || node->type == SUB) && RootType == MUL)
            fprintf(f_sav, ")");
    }
}

void TreePrint (Node* root, FILE* f_out) {
    assert(root);
    assert(f_out);


    fprintf (f_out, "digraph {\n");

    NodePrint(root, f_out);
    PrintNods (root, f_out);

    fprintf(f_out, "}\n");
    fclose(f_out);
}

void NodePrint (Node* node, FILE* f_out) {
    if (node->data)
        fprintf (f_out, "node%p [label=\"%s\", shape=box];\n", node, node->data);
    else
        fprintf (f_out, "node%p [label=\"%.2lf\", shape=box];\n", node, node->num);

}


void PrintNods (Node* node, FILE* f_out) {
    assert (node);
    assert (f_out);

    if (node->left != nullptr && node->right != nullptr) {
        NodePrint(node->left, f_out);
        NodePrint(node->right, f_out);

        fprintf (f_out, "node%p -> node%p\n", node, node->left);
        fprintf (f_out, "node%p -> node%p\n", node, node->right);

        PrintNods (node->right, f_out);
        PrintNods (node->left,  f_out);
    }

}
