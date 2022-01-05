#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;
// g++ -o gf grafic.cpp -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

const long double eps = 1e-10;
const long double pi = acos(-1);
const long double e = 2.718281;
const long double inf = 1e10;

bool functionIsValidVar = true;
inline void setBackground()
{
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    readimagefile("ImageBackground.jpg", 0, 0, screenWidth, screenHeigth);
}
struct point
{
    int x, y;
};
struct myspace
{
    point centre;
    int dim, unit;
    int translation_x, translation_y;
    long double pixel, maxy, miny;
} space;
long double pixelvalue(int x, myspace space)
{
    return (x - space.centre.x - space.translation_x) * space.pixel;
}
int currentLanguage = 0; // 0 for Romanian, 1 for English, 2 for French
unordered_map<string, vector<string>> translationForLanguages;
typedef void (*functionD)(int, int, int, int);
void setTextToStandard();
class button
{
public:
    int x1, y1;
    int x2, y2;
    string txt;
    functionD dF;
    button(int xA, int yA, int xB, int yB, const string &text, void (*fct)(int, int, int, int))
    {
        txt = text;
        x1 = xA;
        x2 = xB;
        y1 = yA;
        y2 = yB;
        dF = fct;
    }
    void draw()
    {
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, YELLOW);
        dF(x1, y1, x2, y2);
        if (!txt.empty())
        {
            settextstyle(3, HORIZ_DIR, 3);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            setbkcolor(YELLOW);
            setcolor(BLACK);
            outtextxy((x1 + x2) / 2, (y1 + y2) / 2, (char *)txt.c_str());
            setcolor(WHITE);
            setbkcolor(BLACK);
        }
    }
    bool isPressed()
    {
        POINT pos;
        GetCursorPos(&pos);
        if (x1 <= pos.x && pos.x <= x2 && y1 <= pos.y && pos.y <= y2 && GetAsyncKeyState(VK_LBUTTON))
            return true;
        return false;
    }
};
inline void drawButtons(const vector<button> &v)
{
    for (auto it : v)
        it.draw();
}
class cleverStackChar : public stack<char>
{
public:
    void pop()
    {
        if (!stack<char>::empty())
            stack<char>::pop();
        else
            functionIsValidVar = false;
    }
    char top()
    {
        if (!stack<char>::empty())
            return stack<char>::top();
        else
        {
            functionIsValidVar = false;
            return 0;
        }
    }
};
class cleverStackLDouble : public stack<long double>
{
public:
    void pop()
    {
        if (!stack<long double>::empty())
            stack<long double>::pop();
        else
            functionIsValidVar = false;
    }
    long double top()
    {
        if (!stack<long double>::empty())
            return stack<long double>::top();
        else
        {
            functionIsValidVar = false;
            return 0;
        }
    }
};
void mainMenu();
bool isValidCharacter(char c)
{
    if (isupper(c))
        return false;
    if (isalnum(c) || c == ' ' || c == '*' || c == '/' || c == '+' || c == '-' || c == '^' || c == '(' || c == ')' || c == '<' || c == '>' || c == '=' || c == '#' || c == '.')
        return true;
    return false;
}
bool isOperator(char c)
{
    if (c == '*' || c == '/' || c == '+' || c == '-' || c == '^' || c == '(' || c == ')' || c == '<' || c == '>' || c == '=' || c == '#' || c == '.')
        return true;
    return false;
}
void updateText(int &x, int y, string &eq, char c)
{
    char txtForFunction[105];
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    if (!isValidCharacter(c))
    {
        strcpy(txtForFunction, translationForLanguages["Invalid character"][currentLanguage].c_str());
        outtextxy(screenWidth / 2 - 100, screenHeigth / 2 + 50, txtForFunction);
    }
    eq += c;
    string aux(1, c);
    outtextxy(x, y, (char *)aux.c_str());
    x += 15;
}
void backSpace(int &x, int y, string &eq)
{
    if (!eq.empty())
    {
        string aux(1, eq.back());
        eq.pop_back();
        x -= 15;
        outtextxy(x, y, (char *)aux.c_str());
        setfillstyle(SOLID_FILL, BLACK);
        bar(x - 10, y - 15, x + 15, y + 4);
        setfillstyle(SLASH_FILL, YELLOW);
    }
}
void drawFunction(const string &s);
bool paranthesisAreCorrect(const string &s)
{
    int k = 0;
    bool ok = true;
    for (int i = 0; i < s.size() && ok; i++)
    {
        if (s[i] == '(')
            k++;
        else if (s[i] == ')')
            k--;
        if (k < 0)
            ok = false;
    }
    if (ok && !k)
        return true;
    return false;
}
long double evaluate(const string &s, long double x);
bool isValidFunction(string s)
{
    string::iterator end_pos = remove(s.begin(), s.end(), ' ');
    s.erase(end_pos, s.end());
    for (auto it : s)
        if (!isValidCharacter(it))
            return false;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == 's' && !(i > 1 && ((s[i - 1] == 'o' && s[i - 2] == 'c') || s[i - 1] == 'b' && s[i - 2] == 'a')))
        {
            if (i > s.size() - 3 || s[i + 1] != 'i' || s[i + 2] != 'n')
                return false;
        }
        else if (s[i] == 'c')
        {
            if (i > s.size() - 3 || s[i + 1] != 'o' || s[i + 2] != 's')
                return false;
        }
        else if (s[i] == 't')
        {
            // can be tan or tg
            if ((i > s.size() - 3 || s[i + 1] != 'a' || s[i + 2] != 'n') && (i > s.size() - 2 || s[i + 1] != 'g'))
                return false;
        }
        else if (s[i] == 'l')
        {
            if ((i > s.size() - 3 || s[i + 1] != 'o' || s[i + 2] != 'g') && (i > s.size() - 2 || (s[i + 1] != 'g' && s[i + 1] != 'n')))
                return false;
        }
        else if (s[i] == 'x')
        {
            if (i < s.size() - 1 && (!isOperator(s[i + 1]) || s[i + 1] == '('))
                return false;
        }
        else if (s[i] == ')')
        {
            if (i < s.size() - 1 && (!isOperator(s[i + 1]) || s[i + 1] == '('))
                return false;
        }
        else if ('0' <= s[i] && s[i] <= '9')
        {
            if (i < s.size() - 1 && ((s[i + 1] > '9' || s[i + 1] < '0') && !isOperator(s[i + 1]) || s[i + 1] == '('))
                return false;
        }
        else if (s[i] == 'p')
        {
            if (i > s.size() - 2 || s[i + 1] != 'i')
                return false;
        }
        else if (s[i] == 'e' || (i > 0 && s[i - 1] == 'p' && s[i] == 'i'))
        {
            if (i < s.size() - 1 && (!isOperator(s[i + 1]) || s[i + 1] == '('))
                return false;
        }
    }
    functionIsValidVar = true;
    bool correctParanthesis = paranthesisAreCorrect(s);
    if (!correctParanthesis)
        return false;
    evaluate(s, 1);
    return functionIsValidVar;
}
inline void functionInput()
{
    cleardevice();
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    readimagefile("Blackboard.jpg", 0, 0, screenWidth, screenHeigth);
    string equation;
    char txtForFunction[105];
    strcpy(txtForFunction, translationForLanguages["Write your function here"][currentLanguage].c_str());
    outtextxy(screenWidth / 2 - 200, screenHeigth / 2 - 200, txtForFunction);

    int crtX = screenWidth / 2 - 300, crtY = screenHeigth / 2;
    setfillstyle(SOLID_FILL, BLACK);
    bar(screenWidth / 2 - 350, screenHeigth / 2 - 100, screenWidth / 2 + 300, screenHeigth / 2 + 100);
    setfillstyle(SLASH_FILL, YELLOW);
    while (true)
    {
        while (!kbhit())
            ;

        setfillstyle(SOLID_FILL, BLACK);
        bar(screenWidth / 2 - 250, screenHeigth / 2 + 30, screenWidth / 2 + 50, screenHeigth / 2 + 55);
        char c = getch();
        if (c != 8 && c != 13)
            updateText(crtX, crtY, equation, c);
        else if (c == 8)
            backSpace(crtX, crtY, equation);
        else
        {
            if (isValidFunction(equation))
            {
                drawFunction(equation);
                break;
            }
            else
            {
                strcpy(txtForFunction, translationForLanguages["Invalid function"][currentLanguage].c_str());
                outtextxy(screenWidth / 2 - 100, screenHeigth / 2 + 50, txtForFunction);
            }
        }
    }
    mainMenu();
}
inline void initialize()
{
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenWidth, screenHeigth, "", -3, -3);
    translationForLanguages["Start"] = {"Start", "Start", "Debut"};

    translationForLanguages["Languages"] = {"Limbi", "Languages", "Langues"};

    translationForLanguages["Exit"] = {"Iesire", "Exit", "Quitter"};

    translationForLanguages["Write your function here"].push_back("Scrieti functia dumneavoastra aici");
    translationForLanguages["Write your function here"].push_back("Write your function here.");
    translationForLanguages["Write your function here"].push_back("Ecrivez votre fonction ici.");

    translationForLanguages["Invalid function"].push_back("Functia este invalida");
    translationForLanguages["Invalid function"].push_back("The function is invalid");
    translationForLanguages["Invalid function"].push_back("La fonction n'est pas valide");

    translationForLanguages["Invalid character"] = {"Caracter invalid", "Invalid character", "Caractere non valide"};

    translationForLanguages["Left"] = {"Stanga", "Left", "Gauche"};
    translationForLanguages["Right"] = {"Dreapta", "Right", "Droite"};
    translationForLanguages["Up"] = {"Sus", "Up", "Haut"};
    translationForLanguages["Down"] = {"Jos", "Down", "Bas"};
    translationForLanguages["Center"] = {"Centru", "Center", "Centre"};

    translationForLanguages["Integral"] = {"Integrala: ", "Integral: ", "Integrale: "};
    //setwritemode(XOR_PUT); doesn't work with text so, no use here
}
void changeLanguage(const string &language)
{
    if (language == "Romanian")
        currentLanguage = 0;
    else if (language == "English")
        currentLanguage = 1;
    else if (language == "French")
        currentLanguage = 2;
}
void languagesMenu()
{
    cleardevice();

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    readimagefile("ImageBackground.jpg", 0, 0, screenWidth, screenHeigth);

    int xButtons = screenHeigth / 2;
    button romanianButton(300, xButtons, 400, xButtons + 100, "Romanian", bar);
    button englishButton(500, xButtons, 600, xButtons + 100, "English", bar);
    button frenchButton(700, xButtons, 800, xButtons + 100, "Francais", bar);
    button backButton(900, xButtons, 1000, xButtons + 100, translationForLanguages["Exit"][currentLanguage], bar);
    vector<button> functionButtons = {romanianButton, englishButton, frenchButton, backButton};
    drawButtons(functionButtons);
    while (true)
    {
        if (romanianButton.isPressed())
            changeLanguage("Romanian");
        else if (englishButton.isPressed())
            changeLanguage("English");
        else if (frenchButton.isPressed())
            changeLanguage("French");
        else if (backButton.isPressed())
            break;
    }
    mainMenu();
}
double simpsonIntegration(const string &s, double a, double b)
{
    const int N = 1000; // number of steps
    double h = (b - a) / N;
    double area = 0;
    double value1 = evaluate(s, a), value2 = evaluate(s, b);
    if (!isnan(value1) && isfinite(value1))
        area += value1;
    if (!isnan(value2) && isfinite(value2))
        area += value2;
    for (int i = 1; i <= N - 1; ++i)
    { // Refer to final Simpson's formula
        double x = a + h * i;
        double value = evaluate(s, x);
        if (!isnan(value) && isfinite(value))
            area += evaluate(s, x) * ((i & 1) ? 4 : 2);
    }
    area *= h / 3;
    if (abs(area) < eps)
        area = 0;
    return area;
}
string toString(double x)
{
    ostringstream strs;
    strs << x;
    return strs.str();
}
void evaluateAndDrawIntegral(const string &s, myspace space)
{
    double area = simpsonIntegration(s, pixelvalue(space.centre.x - space.dim, space), pixelvalue(space.centre.x + space.dim, space));

    int spaceBorderX = space.centre.x + space.dim + 200;
    int spaceBorderY = space.centre.y + space.dim - 500;

    string printIntegral = translationForLanguages["Integral"][currentLanguage] + toString(area);
    settextstyle(3, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setfillstyle(SOLID_FILL, BLACK);
    bar(spaceBorderX - 100, spaceBorderY - 100, spaceBorderX + 200, spaceBorderY + 100);
    setcolor(WHITE);
    outtextxy(spaceBorderX, spaceBorderY, (char *)printIntegral.c_str());
}
void mainMenu()
{
    cleardevice();
    setBackground();
    button Start(200, 100, 400, 200, translationForLanguages["Start"][currentLanguage], bar);
    button Languages(200, 200, 400, 300, translationForLanguages["Languages"][currentLanguage], bar);
    button Exit(200, 300, 400, 400, translationForLanguages["Exit"][currentLanguage], bar);
    vector<button> functionButtons = {Start, Languages, Exit};
    drawButtons(functionButtons);
    while (true)
    {
        if (Start.isPressed())
            functionInput();
        else if (Languages.isPressed())
            languagesMenu();
        else if (Exit.isPressed())
            exit(0);
    }
}

void draw_space(myspace space)
{
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    if (abs(space.translation_y) < space.dim)
        line(space.centre.x - space.dim, space.centre.y + space.translation_y, space.centre.x + space.dim, space.centre.y + space.translation_y);

    if (abs(space.translation_x) < space.dim)
        line(space.centre.x + space.translation_x, space.centre.y - space.dim, space.centre.x + space.translation_x, space.centre.y + space.dim);

    rectangle(space.centre.x - space.dim, space.centre.y - space.dim, space.centre.x + space.dim, space.centre.y + space.dim);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    ///x-uri
    for (int i = space.centre.x - space.dim; i <= space.centre.x + space.dim; i++)
        if (abs(i - space.centre.x - space.translation_x) % space.unit == 0)
        {
            line(i, space.centre.y - space.dim, i, space.centre.y - space.dim + 7);
            line(i, space.centre.y + space.dim, i, space.centre.y + space.dim - 7);
        }
    ///y-uri
    for (int i = space.centre.y - space.dim; i <= space.centre.y + space.dim; i++)
        if (abs(i - space.centre.y - space.translation_y) % space.unit == 0)
        {
            line(space.centre.x - space.dim, i, space.centre.x - space.dim + 7, i);
            line(space.centre.x + space.dim, i, space.centre.x + space.dim - 7, i);
        }
}

int sign(long double value)
{
    if (value < 0)
        return 1;
    return -1;
}

int normalizare(long double value)
{
    if (value > space.maxy)
        return space.centre.y - space.dim;

    else if (value < space.miny)
        return space.centre.y + space.dim;

    return space.centre.y + space.dim - round(fabs(value - space.miny) * space.unit);
}

long double number(const string &s, int &i)
{
    long double number = 0;
    while (i < (int)s.size() && '0' <= s[i] && s[i] <= '9')
    {
        number = number * 10 + s[i] - '0';
        i++;
    }
    if (i < (int)s.size() && s[i] == '.')
    {
        long double p = 0.1;
        i++;
        while (i < (int)s.size() && '0' <= s[i] && s[i] <= '9')
        {
            number = number + p * (s[i] - '0');
            i++;
        }
    }
    i--;
    return number;
}

int priority(char op)
{
    if (op < 0)
        return 4;

    if (op == '<' || op == '>' || op == '=' || op == '#')
        return 0;
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;

    return -1;
}

long double produs(long double a, long double b)
{
    if (a == 0 || b == 0)
        return 0;
    else
        return a * b;
}

long double egal(long double a, long double b)
{
    if (isnan(a) || isnan(b) || !isfinite(a) || !isfinite(b))
        return log(-1);

    return fabs(a - b) <= eps;
}

long double mai_mic(long double a, long double b)
{
    if (isnan(a) || isnan(b) || !isfinite(a) || !isfinite(b))
        return log(-1);
    return a < b;
}

long double mai_mare(long double a, long double b)
{
    if (isnan(a) || isnan(b) || !isfinite(a) || !isfinite(b))
        return log(-1);

    return a > b;
}
bool este_functie_cu_un_parametru(char c)
{
    return c == 's' || c == 'c' || c == 't' || c == 'a' || c == 'l';
}

bitset<10> compareResults; // results of compare operations
// this is useful in creating functions with {
int k;
bitset<10> crtCompareResults;
void process_op(cleverStackLDouble &st, char op)
{
    long double r, l;
    r = st.top();
    st.pop();

    if (op < 0)
    {
        switch (-op)
        {
        case '+':
            st.push(r);
            break;
        case '-':
            st.push(-r);
            break;
        }
        return;
    }

    if (!este_functie_cu_un_parametru(op))
    {
        l = st.top();
        st.pop();
    }

    switch (op)
    {
    case '+':
        st.push(l + r);
        break;
    case '-':
        st.push(l - r);
        break;
    case '*':
        st.push(produs(l, r));
        break;
    case '/':
        st.push(l / r);
        break;
    case '^':
        st.push(pow(l, r));
        break;
    case 's':
        st.push(sin(r));
        break;
    case 'c':
        st.push(cos(r));
        break;
    case 't':
        st.push(tan(r));
        break;
    case 'a':
        st.push(fabs(r));
        break;
    case 'l':
        st.push(log(r));
        break;

    case '<':
        st.push(mai_mic(l, r));
        crtCompareResults[k++] = (isnan(mai_mic(l, r)) || mai_mic(l, r));
        break;
    case '>':
        st.push(mai_mare(l, r));
        crtCompareResults[k++] = (isnan(mai_mare(l, r)) || mai_mare(l, r));
        break;

    case '=':
        st.push(egal(l, r));
        crtCompareResults[k++] = (isnan(egal(l, r)) || egal(l, r));
        break;

    case '#':
        if (isnan(egal(l, r)))
            st.push(log(-1));
        else
            st.push(!egal(l, r));
        crtCompareResults[++k] = (isnan(egal(l, r)) || egal(l, r));
        break;
    }
}

long double evaluate(const string &s, long double x)
{
    k = 0;
    compareResults = crtCompareResults;
    cleverStackLDouble st;
    cleverStackChar op;
    bool poate_este_unar = 1;

    for (int i = 0; i < (int)s.size(); i++)
    {
        /// doar spatiu
        if (s[i] == ' ')
            continue;

        /// variabila
        if (s[i] == 'x')
        {
            st.push(x);
            poate_este_unar = 0;
            continue;
        }
        if (s[i] == 'e')
        {
            st.push(e);
            poate_este_unar = 0;
            continue;
        }
        if (s[i] == 'p')
        {
            st.push(pi);
            poate_este_unar = 0;
            i++;
            continue;
        }

        if (este_functie_cu_un_parametru(s[i]))
        {
            op.push(s[i]);
            for (; s[i] != '('; i++)
                ;
            i--;
        }

        else if (s[i] == '(')
        {
            op.push('(');
            poate_este_unar = 1;
        }

        else if (s[i] == ')')
        {
            while (op.top() != '(')
            {
                process_op(st, op.top());
                op.pop();
            }

            op.pop();
            poate_este_unar = 0;

            if (!op.empty() && este_functie_cu_un_parametru(op.top()))
            {
                process_op(st, op.top());
                op.pop();
            }
        }

        else if ('0' <= s[i] && s[i] <= '9')
        {
            st.push(number(s, i));
            poate_este_unar = 0;
        }
        else if (isOperator(s[i]))
        {
            char op_cur = s[i];

            if (poate_este_unar && (op_cur == '+' || op_cur == '-'))
                op_cur = -op_cur;

            while (!op.empty() && ((s[i] != '^' && priority(op.top()) >= priority(op_cur)) || (priority(op.top()) > priority(s[i]))))
            {
                process_op(st, op.top());
                op.pop();
            }
            op.push(op_cur);
            poate_este_unar = 1;
        }
        else
        {
            functionIsValidVar = false;
            return 0; // arbitrarty
        }
    }

    while (!op.empty())
    {
        process_op(st, op.top());
        op.pop();
    }
    return st.top();
}
void printInterval()
{
    int screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    setfillstyle(SOLID_FILL, BLACK);
    bar(150 + 30, screenHeigth / 2 - 350, 350, screenHeigth / 2 - 120);
    double x1 = pixelvalue(space.centre.x - space.dim, space), x2 = pixelvalue(space.centre.x + space.dim, space);
    double y1 = space.miny, y2 = space.maxy;
    string x1S = toString(x1), x2S = toString(x2);
    string y1S = toString(y1), y2S = toString(y2);
    setTextToStandard();
    settextjustify(LEFT_TEXT, CENTER_TEXT);
    outtextxy(150 + 30, screenHeigth / 2 - 300, (char *)x1S.c_str());
    outtextxy(150 + 30, screenHeigth / 2 - 250, (char *)x2S.c_str());
    outtextxy(150 + 30, screenHeigth / 2 - 200, (char *)y1S.c_str());
    outtextxy(150 + 30, screenHeigth / 2 - 150, (char *)y2S.c_str());
}
void reset_graph()
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(space.centre.x - space.dim - 10, space.centre.y - space.dim - 10, space.centre.x + space.dim + 10, space.centre.y + space.dim + 10);
    printInterval();
}

void reEvaluateFunction(const string &s, myspace space, int intervala, int intervalb)
{
    evaluateAndDrawIntegral(s, space);
    setcolor(RED);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    intervala = max(space.centre.x - space.dim, intervala);
    intervalb = min(space.centre.x + space.dim, intervalb);
    for (int punct = intervala; punct < intervalb; punct++)
    {
        long double stvalue = evaluate(s, pixelvalue(punct, space));
        long double drvalue = evaluate(s, pixelvalue(punct + 1, space));

        if (isnan(stvalue) || isnan(drvalue) || fabs(stvalue) >= inf || fabs(drvalue) >= inf)
            continue;

        int y1 = normalizare(stvalue);
        int y2 = normalizare(drvalue);

        if (compareResults == crtCompareResults && fabs(y2 - y1) <= space.dim)
            line(punct, y1, punct + 1, y2);
    }
    draw_space(space);
}

void sweepline(const string &s)
{
    POINT prevmouse, curmouse;
    GetCursorPos(&prevmouse);
    int screenHeigth = GetSystemMetrics(SM_CYSCREEN);

    outtextxy(150, screenHeigth / 2, (char *)"x = ");
    outtextxy(150, screenHeigth / 2 + 50, (char *)"y = ");
    while (true)
    {
        GetCursorPos(&curmouse);
        delay(100);

        if (curmouse.x != prevmouse.x)
        {
            setfillstyle(SOLID_FILL, BLACK);
            bar(prevmouse.x - 7, space.centre.y - space.dim, prevmouse.x + 7, space.centre.y + space.dim);
            bar(150 + 30, screenHeigth / 2 - 100, 350, screenHeigth / 2 + 150);

            reEvaluateFunction(s, space, prevmouse.x - 10, prevmouse.x + 10);

            prevmouse = curmouse;
            if (!(space.centre.x - space.dim < prevmouse.x && space.centre.x + space.dim > prevmouse.x &&
                  space.centre.y - space.dim < prevmouse.y && space.centre.y + space.dim > prevmouse.y))
                return;

            setcolor(GREEN);
            setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
            line(prevmouse.x, space.centre.y - space.dim, prevmouse.x, space.centre.y + space.dim);
            line(prevmouse.x + 1, space.centre.y - space.dim, prevmouse.x + 1, space.centre.y + space.dim);

            long double pointvalue = evaluate(s, pixelvalue(prevmouse.x, space));

            string valuesToShowX = toString(pixelvalue(prevmouse.x, space));
            string valuesToShowY = toString(pointvalue);
            if (isnan(pointvalue) || isnan(pointvalue) || fabs(pointvalue) > inf || fabs(pointvalue) > inf || pointvalue < space.miny || pointvalue > space.maxy)
                valuesToShowY = "NaN";
            else
            {
                setcolor(GREEN);
                int centre = normalizare(pointvalue);
                for (int i = 1; i <= 5; i++)
                    circle(prevmouse.x, centre, i);
            }
            setTextToStandard();
            settextjustify(LEFT_TEXT, CENTER_TEXT);
            outtextxy(150 + 30, screenHeigth / 2, (char *)valuesToShowX.c_str());
            outtextxy(150 + 30, screenHeigth / 2 + 50, (char *)valuesToShowY.c_str());
        }
    }
}
void setTextToStandard()
{
    settextstyle(3, HORIZ_DIR, 3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setbkcolor(BLACK);
    setcolor(WHITE);
}
void printFunctionOverGraphic(const string &s, myspace space)
{
    setTextToStandard();
    int spaceBorderYUp = space.centre.y - space.dim - 20;
    const string toPrint = "f(x) = " + s;
    outtextxy(space.centre.x, spaceBorderYUp, (char *)toPrint.c_str());
}
void drawFunction(const string &s)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeigth = GetSystemMetrics(SM_CYSCREEN);

    space.unit = 30;
    space.centre = {screenWidth / 2, screenHeigth / 2};
    space.dim = screenHeigth / 2 - 100;
    space.pixel = (long double)1 / space.unit;
    space.maxy = (long double)space.dim / space.unit;
    space.miny = -space.maxy;
    space.translation_x = 0;
    space.translation_y = 0;
    initwindow(screenWidth, screenHeigth, "", -3, -3);
    printFunctionOverGraphic(s, space);
    outtextxy(150, screenHeigth / 2 - 300, (char *)"x1 = ");
    outtextxy(150, screenHeigth / 2 - 250, (char *)"x2 = ");
    outtextxy(150, screenHeigth / 2 - 200, (char *)"y1 = ");
    outtextxy(150, screenHeigth / 2 - 150, (char *)"y2 = ");
    printInterval();
    int spaceBorderX = space.centre.x + space.dim + 20;
    int spaceBorderY = space.centre.y + space.dim - 100;

    // draws plus and minus buttons
    auto drawPlus = [](int x1, int y1, int x2, int y2)
    {
        int sz = max(y2 - y1, x2 - x1) / 5;
        bar(x1, (y2 + y1) / 2 - sz / 2, x2, (y2 + y1) / 2 + sz / 2);
        bar((x2 + x1) / 2 - sz / 2, y1, (x2 + x1) / 2 + sz / 2, y2);
    };
    auto drawMinus = [](int x1, int y1, int x2, int y2)
    {
        int sz = max(y2 - y1, x2 - x1) / 5;
        bar(x1, (y2 + y1) / 2 - sz / 2, x2, (y2 + y1) / 2 + sz / 2);
    };

    button plusButton(spaceBorderX, spaceBorderY, spaceBorderX + 50, spaceBorderY + 50, "", drawPlus);
    button minusButton(spaceBorderX + 100, spaceBorderY, spaceBorderX + 150, spaceBorderY + 50, "", drawMinus);
    button exitButton(spaceBorderX + 200, spaceBorderY, spaceBorderX + 300, spaceBorderY + 100,
                      translationForLanguages["Exit"][currentLanguage], bar);

    button leftButton(spaceBorderX, spaceBorderY - 110, spaceBorderX + 100, spaceBorderY - 10,
                      translationForLanguages["Left"][currentLanguage], bar);
    button rightButton(spaceBorderX + 110, spaceBorderY - 110, spaceBorderX + 210, spaceBorderY - 10,
                       translationForLanguages["Right"][currentLanguage], bar);
    button upButton(spaceBorderX, spaceBorderY - 220, spaceBorderX + 100, spaceBorderY - 120,
                    translationForLanguages["Up"][currentLanguage], bar);
    button downButton(spaceBorderX + 110, spaceBorderY - 220, spaceBorderX + 210, spaceBorderY - 120,
                      translationForLanguages["Down"][currentLanguage], bar);
    button recentreButton(spaceBorderX + 220, spaceBorderY - 110, spaceBorderX + 320, spaceBorderY - 10,
                          translationForLanguages["Center"][currentLanguage], bar);

    reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);

    vector<button> functionButtons = {plusButton, minusButton, exitButton, leftButton, rightButton, recentreButton, upButton, downButton};
    drawButtons(functionButtons);

    while (true)
    {
        if (exitButton.isPressed())
            return;

        POINT mouse;
        GetCursorPos(&mouse);
        if (space.centre.x - space.dim < mouse.x && space.centre.x + space.dim > mouse.x &&
            space.centre.y - space.dim < mouse.y && space.centre.y + space.dim > mouse.y)
            sweepline(s);

        else if (plusButton.isPressed() && space.translation_x == 0 && space.translation_y == 0)
        {
            space.unit += 10;
            space.pixel = (long double)1 / space.unit;
            space.maxy = (long double)space.dim / space.unit;
            space.miny = -space.maxy;
            reset_graph();
            reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);
        }

        else if (minusButton.isPressed() && space.unit > 20 && space.translation_x == 0 && space.translation_y == 0)
        {
            space.unit -= 10;
            space.pixel = (long double)1 / space.unit;
            space.maxy = (long double)space.dim / space.unit;
            space.miny = -space.maxy;
            reset_graph();
            reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);
        }

        else if (leftButton.isPressed())
        {
            space.translation_x += 10;
            reset_graph();
            reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);
        }
        else if (rightButton.isPressed())
        {
            space.translation_x -= 10;
            reset_graph();
            reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);
        }
        else if (upButton.isPressed())
        {
            space.translation_y += 10;
            space.maxy = space.maxy + 10 * space.pixel;
            space.miny = space.miny + 10 * space.pixel;
            reset_graph();
            reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);
        }
        else if (downButton.isPressed())
        {
            space.translation_y -= 10;
            space.maxy = space.maxy - 10 * space.pixel;
            space.miny = space.miny - 10 * space.pixel;
            reset_graph();
            reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);
        }
        else if (recentreButton.isPressed())
        {
            space.translation_x = 0;
            space.translation_y = 0;
            space.maxy = (long double)space.dim / space.unit;
            space.miny = -space.maxy;
            reset_graph();
            reEvaluateFunction(s, space, space.centre.x - space.dim, space.centre.x + space.dim);
        }
    }
}
int main()
{
    initialize();
    mainMenu();

    return 0;
}
