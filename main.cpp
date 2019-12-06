#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define _SUM(left, right) CreateNode (SUM, "+", left, right)
#define _SUB(left, right) CreateNode (SUB, "-", left, right)
#define _MUL(left, right) CreateNode (MUL, "*", left, right)
#define _DIV(left, right) CreateNode (DIV, "/", left, right)
#define _POW(left, right) CreateNode (POW, "^", left, right)
#define _VAR CreateNode (VAR, "x", nullptr, nullptr)
#define _NUM(num) CreateNode (num)
#define _SIN(right) CreateNode (SIN, "sin", nullptr, right)
#define _COS(right) CreateNode (COS, "cos", nullptr, right)
#define _TG(right) CreateNode (TG, "tg", nullptr, right)
#define _CTG(right) CreateNode (CTG, "ctg", nullptr, right)
#define _ARCTG(right) CreateNode (ARCTG, "arctg", nullptr, right)
#define _LN(right) CreateNode (LN, "ln", nullptr, right)
#define _SH(right) CreateNode (SH, "sh", nullptr, right)
#define _CH(right) CreateNode (CH, "ch", nullptr, right)
#define _TH(right) CreateNode (TH, "th", nullptr, right)
#define _CTH(right) CreateNode (CTH, "cth", nullptr, right)
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
    POW,
    DIV,
    VAR,
    SIN,
    COS,
    TG,
    CTG,
    ARCTG,
    ARCCTG,
    LN,
    SH,
    CH,
    TH,
    CTH
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
Node* GetStep ();
Node* GetP ();
Node* GetF ();
Node* GetN ();



Node* CreateNode (int type, char* data, Node* left, Node* right);
Node* CreateNode (double num);
Node* CopyNode (Node* root);
Node* DifNode (const Node* node);
Node* DelNode (Node* node);
void DeleteTree (Node* root);
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
    //Node* root = GetG ("x^2+4.5*x-tg(x)");
    Node* root = GetG ("cth(x^2)");
    Simplification (root);
    Node* d_root = DifNode (root);
    Simplification (d_root);
    TreePrint (root, f_out);

    FILE* f_tex = fopen ("F:\\LaTex\\output.tex", "w");
    assert (f_tex);

    TreeToLaTex (root, d_root, f_tex);
    DeleteTree (root);
    DeleteTree (d_root);
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
    val = GetStep ();
    while (*s == '*' || *s == '/') {
        char op = *s;
        s++;
        Node* val2 = GetStep ();
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

Node* GetStep () {
    Node* val = nullptr;
    val = GetP ();
    while (*s == '^') {
        s++;
        Node* val2 = GetP ();
        val = _POW (val, val2);
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
        return _VAR;
    }
    else if ('0' <= *s && *s <= '9')
        return GetN ();
    else
        return GetF ();
}

Node* GetF () {

    Node* val = nullptr;
    if (strncmp (s, "sin", 3) == 0) {
        s += 3;
        val = GetP ();
        return _SIN (val);
    }

    if (strncmp (s, "cos", 3) == 0) {
        s += 3;
        val = GetP ();
        return _COS (val);
    }

    if (strncmp (s, "tg", 2) == 0) {
        s += 2;
        val = GetP ();
        return _TG (val);
    }

    if (strncmp (s, "ctg", 3) == 0) {
        s += 3;
        val = GetP ();
        return _CTG (val);
    }

    if (strncmp (s, "ln", 2) == 0) {
        s += 2;
        val = GetP ();
        return _LN (val);
    }

    if (strncmp (s, "arctg", 5) == 0) {
        s += 5;
        val = GetP ();
        return _ARCTG (val);
    }

    if (strncmp (s, "sh", 2) == 0) {
        s += 2;
        val = GetP ();
        return _SH (val);
    }

    if (strncmp (s, "ch", 2) == 0) {
        s += 2;
        val = GetP ();
        return _CH (val);
    }

    if (strncmp (s, "th", 2) == 0) {
        s += 2;
        val = GetP ();
        return _TH (val);
    }

    if (strncmp (s, "cth", 3) == 0) {
        s += 3;
        val = GetP ();
        return _CTH (val);
    }
//functions

    return val;
}

Node* GetN () {
    Node* res = nullptr;
    double val = 0.0;
    int n = 0;

    sscanf (s, "%lf%n", &val, &n);
    s += n;
    res = _NUM (val);
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
if (root) {
    if (_L && _R) {
        Simplification (_L);
        Simplification (_R);
        Node* NewNode = nullptr;
        if (_L->type == NUM && _R->type == NUM) {
            switch (root->type) {
                case SUM:
                    NewNode = _NUM (_L->num + _R->num);
                    CopyTo (root, NewNode);
                    break;
                case SUB:
                    NewNode = _NUM (_L->num - _R->num);
                    CopyTo (root, NewNode);
                    break;
                case MUL:
                    NewNode = _NUM (_L->num * _R->num);
                    CopyTo (root, NewNode);
                    break;
                case DIV:
                    if (_R->num) {
                        NewNode = _NUM (_L->num / _R->num);
                        CopyTo (root, NewNode);
                    }
                    break;
                case POW:
                    assert (_L->num == 0);
                    NewNode = _NUM (pow (_L->num, _R->num));
                    CopyTo (root, NewNode);
            }
        }

        if (root->type == MUL) {
            if ((_L->type == NUM && _L->num == 0) || (_R->type == NUM && _R->num == 0)) {
                NewNode = _NUM (0);
                CopyTo (root, NewNode);
            } else if (_L->type == NUM && _L->num == 1) {
                CopyTo (root, _R);
            } else if (_R->type == NUM && _R->num == 1) {
                CopyTo (root, _L);
            }
            else if (_L->type == _R->type && _L->type == VAR) {
                NewNode = _POW (_L, _NUM (2));
                CopyTo (root, NewNode);
            }
        }

        if (root->type == SUM || root->type == SUB) {
            if (_L->type == NUM && _L->num == 0) {
                CopyTo (root, _R);
            } else if (_R->type == NUM && _R->num == 0) {
                CopyTo (root, _L);
            }
        }

        if (root->type == POW) {
            if (_R->type == NUM && _R->num == 0) {
                NewNode = _NUM (1);
                CopyTo (root, NewNode);
            } else if (_L->type == NUM && _L->num == 1) {
                NewNode = _NUM (1);
                CopyTo (root, NewNode);
            } else if (_R->type == NUM && _R->num == 1) {
                CopyTo (root, _L);
            }
        }

        if (NewNode) free (NewNode);
    }
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
                return _NUM (0);
            case VAR:
                return _NUM (1);
            case SUM:
                return _SUM (dL, dR);
            case SUB:
                return _SUB (dL, dR);
            case MUL:
                return _SUM (_MUL (dL, cR), _MUL (cL, dR));
            case DIV:
                return _DIV (_SUB (_MUL (dL, cR), _MUL (cL, dR)), _MUL (cR, cR));
            case SIN:
                return _MUL (_COS (cR), dR);
            case COS:
                return _MUL (_NUM (-1), _MUL (_SIN (cR), dR));
            case TG:
                return _MUL (_DIV (_NUM (1), _MUL (_COS (cR), _COS (cR))), dR);
            case CTG:
                return _MUL (_DIV (_NUM (-1), _MUL (_SIN (cR), _SIN (cR))), dR);
            case LN:
                return _MUL (_DIV (_NUM(1), cR), dR);
            case ARCTG:
                return _MUL (_DIV (_NUM (1), _SUM (_NUM(1), _MUL (cR, cR))), dR);
            case POW:
                return _MUL (_MUL (cR, _POW (cL, _SUB (cR, _NUM (1)))), dL);
            case SH:
                return _MUL (_CH (cR), dR);
            case CH:
                return _MUL (_SH (cR), dR);
            case TH:
                return _MUL (_DIV (_NUM (1), _MUL (_CH (cR), _CH (cR))), dR);
            case CTH:
                return _MUL (_DIV (_NUM (-1), _MUL (_SH (cR), _SH (cR))), dR);
        }
}

Node* CopyNode (Node* root) {

if (root) {
    switch (root->type) {
        case NUM:
            return  _NUM (root->num);
        case VAR:
            return  _VAR;
        case MUL:
            return  _MUL (CopyNode (_L), CopyNode (_R));
        case DIV:
            return _DIV (CopyNode (_L), CopyNode (_R));
        case SUM:
            return _SUM (CopyNode (_L), CopyNode (_R));
        case SUB:
            return _SUB (CopyNode (_L), CopyNode (_R));
        case SIN:
            return _SIN (CopyNode (_R));
        case COS:
            return _COS (CopyNode (_R));
        case TG:
            return _TG (CopyNode (_R));
        case CTG:
            return _CTG (CopyNode (_R));
        case LN:
            return _LN (CopyNode (_R));
        case ARCTG:
            return _ARCTG (CopyNode (_R));
        case POW:
            return _POW (CopyNode (_L), CopyNode (_R));
        case SH:
            return _SH (CopyNode (_R));
        case CH:
            return _CH (CopyNode (_R));
        case TH:
            return _TH (CopyNode (_R));
        case CTH:
            return _CTH (CopyNode (_R));
        default:
            return nullptr;
    }
}
}

Node* DelNode (Node* node) {
    free (node->data);
    free (node->left);
    free (node->right);
}

void DeleteTree (Node* root) {

    if (root) {
        DeleteTree (_L);
        DeleteTree (_R);
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
    SaveTree (root, 0, f_tex);
    fprintf (f_tex, "$:\n"
                    "\\[");
    SaveTree (d_root, 0, f_tex);
    fprintf (f_tex,"\\]\n"
                   "\\end{document}");

    fclose (f_tex);
}

void SaveTree (Node* node, int RootType, FILE* f_sav) {
    if (node) {
        if (((node->type == SUM || node->type == SUB) && (RootType == MUL  || RootType == POW) )
            || RootType > 6
            ||  ((node->type == MUL || node->type == DIV) && RootType == POW)) {
            fprintf (f_sav, "(");
        }

        if (node->type == DIV) {
            fprintf (f_sav, "\\frac {");
        }

        SaveTree (node->left, node->type, f_sav);

        if (node->type == DIV) {
            fprintf (f_sav, "}");
        }

        if (node->type != DIV) {
            switch (node->type) {
                case NUM:
                    if (abs (node->num  - (int) node->num) < 0.00001)
                        fprintf (f_sav, "%d", (int) node->num);
                    else
                        fprintf (f_sav, "%.2lf", node->num);
                    break;
                case MUL:
                    fprintf(f_sav, "\\cdot ");
                    break;
                default:
                    fprintf(f_sav, "%s", node->data);
            }
        }

        if (node->type == DIV || node->type == POW) {
            fprintf (f_sav, "{");
        }

        SaveTree (node->right, node->type, f_sav);

        if (node->type == DIV || node->type == POW) {
            fprintf (f_sav, "}");
        }

        if (((node->type == SUM || node->type == SUB) && (RootType == MUL  || RootType == POW) )
            || RootType > VAR
            ||  ((node->type == MUL || node->type == DIV) && RootType == POW)) {
            fprintf (f_sav, ")");
        }

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

        if (node->left != nullptr) NodePrint (node->left, f_out);
        if (node->right != nullptr) NodePrint (node->right, f_out);

        if (node->left != nullptr) fprintf (f_out, "node%p -> node%p\n", node, node->left);
        if (node->right != nullptr) fprintf (f_out, "node%p -> node%p\n", node, node->right);

        if (node->right != nullptr) PrintNods (node->right, f_out);
        if (node->left != nullptr) PrintNods (node->left,  f_out);


}
