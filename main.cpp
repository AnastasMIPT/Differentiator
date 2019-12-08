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
    FUNCCOL,
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



Node* CreateNode (int type, const char* data, Node* left, Node* right);
Node* CreateNode (double num);
Node* CopyNode (Node* root);
Node* DifNode (const Node* node);
void DelNode (Node* node);
void DeleteTree (Node* root);
void PrintNods (Node* node, FILE* f_out);
void NodePrint (Node* node, FILE* f_out);
void TreePrint (Node* root, FILE* f_out);
void SaveTree (Node* node, int RootType, FILE* f_sav);
void TreeToLaTex (Node* root, Node* d_root, FILE* f_tex);
void Simplification (Node* root);
void CopyTo (Node* root, Node* NewNode);
Node* NDifNode (Node* root, int number);
void VarToNum (Node* root, double num);
bool MaklorenElement (Node* root, int num, FILE* f_out);
void MaklorenSeries (Node* root, int num, FILE* f_out);

Node* operator+ (Node a, Node b) {
    return CreateNode (SUM, "+", (&(a)), (&(b)));
}

int main () {
    FILE* f_out = fopen ("F:\\Graphs\\output.dot", "w");
    //Node* root = GetG ("tg(x)^10");
    Node* root = GetG ("cos(x)*ln(1+x)");

    //VarToNum (root, 0);
    Simplification (root);
    Node* d_root = NDifNode (root, 1);
    Simplification (d_root);
    //VarToNum (root, 2.4);
    //Simplification (root);
    TreePrint (d_root, f_out);

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

Node* CreateNode (int type, const char* data, Node* left, Node* right) {

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
        Node* NewNode = nullptr;
        if (_L && _R) {
            Simplification(_L);
            Simplification(_R);
            if (_L->type == NUM && _R->type == NUM) {
                switch (root->type) {
                    case SUM:
                        NewNode = _NUM (_L->num + _R->num);
                        CopyTo(root, NewNode);
                        break;
                    case SUB:
                        NewNode = _NUM (_L->num - _R->num);
                        CopyTo(root, NewNode);
                        break;
                    case MUL:
                        NewNode = _NUM (_L->num * _R->num);
                        CopyTo(root, NewNode);
                        break;
                    case DIV:
                        if (_R->num) {
                            NewNode = _NUM (_L->num / _R->num);
                            CopyTo(root, NewNode);
                        }
                        break;
                    case POW:
                        //assert (_L->num == 0);
                        NewNode = _NUM (pow(_L->num, _R->num));
                        CopyTo(root, NewNode);
                }
            }

            switch (root->type) {
                case MUL:
                    if ((_L->type == NUM && _L->num == 0) || (_R->type == NUM && _R->num == 0)) {
                        NewNode = _NUM (0);
                        CopyTo (root, NewNode);
                    } else if  (_L->type == NUM && _L->num == 1) {
                        CopyTo (root, _R);
                    } else if (_R->type == NUM && _R->num == 1) {
                        CopyTo (root, _L);
                    } else if (_L->type == _R->type && (_L->type == VAR || _L->type > FUNCCOL)) {
                        NewNode = _POW (_L, _NUM(2));
                        CopyTo (root, NewNode);
                    }
                    break;
                case DIV:
                    if (_L->type == NUM && _L->num == 0) {
                        NewNode = _NUM (0);
                        CopyTo (root, NewNode);
                    } else if (_R->type == NUM && _R->num == 1) {
                        CopyTo (root, _L);
                    }
                    break;
                case SUM:
                    if (_L->type == NUM && _L->num == 0) {
                        CopyTo (root, _R);
                    } else if (_R->type == NUM && _R->num == 0) {
                        CopyTo (root, _L);
                    }
                    break;
                case SUB:
                    if (_L->type == NUM && _L->num == 0) {
                        NewNode = _MUL (_NUM (-1), _R);
                        CopyTo (root, NewNode);
                    } else if (_R->type == NUM && _R->num == 0) {
                        CopyTo (root, _L);
                    }
                    break;
                case POW:
                    if (_R->type == NUM && _R->num == 0) {
                        NewNode = _NUM (1);
                        CopyTo (root, NewNode);
                    } else if (_L->type == NUM && _L->num == 1) {
                        NewNode = _NUM (1);
                        CopyTo (root, NewNode);
                    } else if (_R->type == NUM && _R->num == 1) {
                        CopyTo (root, _L);
                    } else if (_L->type == POW) {
                        NewNode = _POW (_L->left, _NUM(_L->right->num * _R->num));
                        CopyTo (root, NewNode);
                    }
                    break;
            }


            } else if (_R) {
            Simplification(_R);
            if ( _R->type == NUM) {
                switch (root->type) {
                    case SIN:
                        NewNode = _NUM (sin(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case COS:
                        NewNode = _NUM (cos(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case TG:
                        NewNode = _NUM (tan(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case CTG:
                        NewNode = _NUM (1 / tan(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case SH:
                        NewNode = _NUM (sinh(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case CH:
                        NewNode = _NUM (cosh(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case TH:
                        NewNode = _NUM (tanh(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case CTH:
                        NewNode = _NUM (1 / tanh(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case ARCTG:
                        NewNode = _NUM (atan(_R->num));
                        CopyTo(root, NewNode);
                        break;
                    case LN:
                        NewNode = _NUM (log(_R->num));
                        CopyTo(root, NewNode);
                        break;
                }
            }
            }

            //if (NewNode) free (NewNode);
        }

    }

void CopyTo (Node* root, Node* NewNode) {

    _L = NewNode->left;
    _R = NewNode->right;
    root->num = NewNode->num;
    root->type = NewNode->type;
    root->data = NewNode->data;
}

void VarToNum (Node* root, double num) {
    if (root) {
        VarToNum (_L, num);
        VarToNum (_R, num);
        if (root->type == VAR) {
            root->type = NUM;
            root->num = num;
        }
    }
}

bool MaklorenElement (Node* root, int num, FILE* f_out) {

    Node* root_copy = CopyNode (root);
    if (num != 0) {
        Node* d_root = NDifNode (root_copy, num);
        VarToNum (d_root, 0);
        Simplification (d_root);

        if (d_root->type == NUM && d_root->num == 0) {
            DeleteTree (root_copy);
            DeleteTree (d_root);
            return false;
        }
        fprintf(f_out, "\\frac {");
        SaveTree (d_root, 0, f_out);
        fprintf (f_out, " \\cdot x^{%d}} {%d!} ", num, num);
        DeleteTree (d_root);
    }
    else
    {
        VarToNum (root_copy, 0);
        Simplification (root_copy);

        if (root_copy->num == 0) {
            DeleteTree (root_copy);
            return false;
        }
        if (abs (root_copy->num  - (int) root_copy->num) < 0.00001)
            fprintf (f_out, "%d", (int) root_copy->num);
        else
            fprintf (f_out, "%.2lf", root_copy->num);
    }
    DeleteTree (root_copy);
    return true;
}

void MaklorenSeries (Node* root, int num, FILE* f_out) {
    fprintf (f_out, "{\\Large \\begin{center}\n"
                    "\\begin{math}\n"
                    "f(x)=");

    for (int i = 0; i < num + 1; i++) {
        if (MaklorenElement (root, i, f_out))
            fprintf (f_out, "+");
    }
    fprintf (f_out, "o(x^{%d})\n", num);
    fprintf (f_out, "\\end{math}\n"
                    "\\end{center}}\n");
}


Node* NDifNode (Node* root, int number) {

    Node* d_root = root;

    for (int i = 0; i < number; i++) {
        Simplification (d_root);
        d_root = DifNode (d_root);
    }

    Simplification (d_root);
    return d_root;
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
                return _DIV (_SUB (_MUL (dL, cR), _MUL (cL, dR)), _POW (cR, _NUM (2)));
            case SIN:
                return _MUL (_COS (cR), dR);
            case COS:
                return _MUL (_NUM (-1), _MUL (_SIN (cR), dR));
            case TG:
                return _DIV (dR, _POW (_COS (cR), _NUM (2)));
            case CTG:
                return _DIV (_MUL (dR, _NUM (-1)), _POW (_SIN (cR), _NUM (2)));
            case LN:
                return _DIV (dR, cR);
            case ARCTG:
                return _DIV (dR, _SUM (_NUM(1), _POW (cR, _NUM (2))));
            case POW:
                return _MUL (_MUL (cR, _POW (cL, _SUB (cR, _NUM (1)))), dL);
            case SH:
                return _MUL (_CH (cR), dR);
            case CH:
                return _MUL (_SH (cR), dR);
            case TH:
                return _DIV (dR, _POW (_CH (cR), _NUM (2)));
            case CTH:
                return _DIV (_MUL (dR, _NUM (-1)), _POW (_SH (cR), _NUM (2)));


        }
    return nullptr;
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
    return nullptr;
}

void DelNode (Node* node) {
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
                    "\\subsection{Решим простой пример}\n"
                    "\n"
                    "{\\Large Найти производную функции \\\\ $f(x)=");
    SaveTree(root, 0, f_tex);
    fprintf (f_tex, "$:\n"
                    "\\begin{center}\n"
                    "\\begin{math}\n"
                    "f^\\prime(x)=");
    SaveTree(d_root, 0, f_tex);
    fprintf (f_tex,"\n"
                   "\\end{math}\n"
                   "\\end{center}\n"
                   "}\n");
    MaklorenSeries (root, 5, f_tex);
    fprintf (f_tex, "\\end{document}");

    fclose (f_tex);
}

void SaveTree (Node* node, int RootType, FILE* f_sav) {
    if (node) {
        if (((node->type == SUM || node->type == SUB) && (RootType == MUL  || RootType == POW) )
            || RootType > FUNCCOL
            || (RootType == POW && node->type > FUNCCOL)
            || ((node->type == MUL || node->type == DIV) && RootType == POW)
            || (node->type == NUM && node->num < 0)) {
            fprintf (f_sav, "(");
        }

        if (node->type == DIV) {
            fprintf (f_sav, "\\frac {");
        }

        SaveTree (node->left, node->type, f_sav);

        if (node->type == DIV) {
            fprintf (f_sav, "}");
        }


        if (node->type != DIV ) {
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
            || RootType > FUNCCOL
            || (RootType == POW && node->type > FUNCCOL)
            || ((node->type == MUL || node->type == DIV) && RootType == POW)
            || (node->type == NUM && node->num < 0)) {
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
