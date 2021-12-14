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
int currentLanguage = 0; // 0 for Romanian, 1 for English, 2 for French
unordered_map<string, vector<string>> translationForLanguages;
typedef void (*functionD)(int, int, int, int);
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
        setfillstyle(SLASH_FILL, YELLOW);
        dF(x1, y1, x2, y2);
        if (!txt.empty())
        {
            settextstyle(3, HORIZ_DIR, 3);
            settextjustify(CENTER_TEXT, CENTER_TEXT);
            outtextxy((x1 + x2) / 2, (y1 + y2) / 2, (char *)txt.c_str());
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
    if (isalnum(c) || c == ' ' || c == '*' || c == '+' || c == '-' || c == '^' || c == '(' || c == ')' || c == '<' || c == '>' || c == '=' || c == '#')
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
    };
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
        bar(x - 10, y - 15, x + 15, y + 3);
        setfillstyle(SLASH_FILL, YELLOW);
    }
}
void drawFunction(string s);
bool paranthesisAreCorrect(string s)
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
bool isValidFunction(const string &s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == 's')
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
    }
    functionIsValidVar = true;
    evaluate(s, 1);
    return (functionIsValidVar && paranthesisAreCorrect(s));
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
        };
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
    translationForLanguages["Center"] = {"Centru", "Center", "Centre"};
    //setwritemode(XOR_PUT); doesn't work with text so, no use here
}
void changeLanguage(const string& language)
{
    if (language == "Romanian")
        currentLanguage = 0;
    else if (language == "English")
        currentLanguage = 1;
    else if (language == "French")
        currentLanguage = 2;
    else
        throw(__throw_bad_function_call);
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
    romanianButton.draw();
    englishButton.draw();
    frenchButton.draw();
    backButton.draw();
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
void mainMenu()
{
    cleardevice();
    setBackground();
    button Start(200, 100, 400, 200, translationForLanguages["Start"][currentLanguage], bar);
    Start.draw();
    button Languages(200, 200, 400, 300, translationForLanguages["Languages"][currentLanguage], bar);
    Languages.draw();
    button Exit(200, 300, 400, 400, translationForLanguages["Exit"][currentLanguage], bar);
    Exit.draw();
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
struct point
{
    int x, y;
};

struct myspace
{
    point centre;
    int dim, unit;
    int translation_x, translation_y;
    long double pixel, maxy;
} space;

void drawPlus(int x1, int y1, int x2, int y2)
{
    int sz = max(y2 - y1, x2 - x1) / 5;
    bar(x1, (y2 + y1) / 2 - sz / 2, x2, (y2 + y1) / 2 + sz / 2);
    bar((x2 + x1) / 2 - sz / 2, y1, (x2 + x1) / 2 + sz / 2, y2);
}
void drawMinus(int x1, int y1, int x2, int y2)
{
    int sz = max(y2 - y1, x2 - x1) / 5;
    bar(x1, (y2 + y1) / 2 - sz / 2, x2, (y2 + y1) / 2 + sz / 2);
}
void draw_space(myspace space)
{
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    rectangle(space.centre.x - space.dim, space.centre.y - space.dim, space.centre.x + space.dim, space.centre.y + space.dim);

    /// axele de coordonate
    line(space.centre.x - space.dim, space.centre.y, space.centre.x + space.dim, space.centre.y);

    if (abs(space.translation_x) < space.dim)
        line(space.centre.x + space.translation_x, space.centre.y - space.dim, space.centre.x + space.translation_x, space.centre.y + space.dim);

    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);

    /// dracu stie cum o fac pe asta
    for (int i = 0; i <= space.dim; i += space.unit)
    {
        ///y-uri
        line(space.centre.x - space.dim, space.centre.y - i, space.centre.x - space.dim + 7, space.centre.y - i);
        line(space.centre.x - space.dim, space.centre.y + i, space.centre.x - space.dim + 7, space.centre.y + i);
        line(space.centre.x + space.dim, space.centre.y - i, space.centre.x + space.dim - 7, space.centre.y - i);
        line(space.centre.x + space.dim, space.centre.y + i, space.centre.x + space.dim - 7, space.centre.y + i);
    }
    for (int i = space.centre.x - space.dim; i <= space.centre.x + space.dim; i++)
        if (abs(i - space.centre.x - space.translation_x) % space.unit == 0)
        {
            line(i, space.centre.y - space.dim, i, space.centre.y - space.dim + 7);
            line(i, space.centre.y + space.dim, i, space.centre.y + space.dim - 7);
        }
}

long double pixelvalue(int x, myspace space)
{
    return (x - space.centre.x - space.translation_x) * space.pixel;
}

int sign(long double value)
{
    if (value < 0)
        return 1;
    return -1;
}

int normalizare(long double value)
{
    if (fabs(value) > space.maxy)
        return space.centre.y + sign(value) * space.dim;

    return space.centre.y + round(sign(value) * fabs(value) * space.unit);
}

long double number(string s, int &i)
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
        else
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
    }

    while (!op.empty())
    {
        process_op(st, op.top());
        op.pop();
    }
    return st.top();
}
void reEvaluateFunction(const string& s, myspace space)
{
    cleardevice();
    setcolor(RED);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    for (int punct = space.centre.x - space.dim; punct < space.centre.x + space.dim; punct++)
    {
        long double stvalue = evaluate(s, pixelvalue(punct, space));
        long double drvalue = evaluate(s, pixelvalue(punct + 1, space));

        if (isnan(stvalue) || isnan(drvalue) || fabs(stvalue) > inf || fabs(drvalue) > inf)
            continue;

        int y1 = normalizare(stvalue);
        int y2 = normalizare(drvalue);

        if (compareResults == crtCompareResults)
            line(punct, y1, punct + 1, y2);
    }
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
    draw_space(space);
}
inline void drawButtons(const vector<button> &v)
{
    for (auto it : v)
        it.draw();
}
void drawFunction(string s)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeigth = GetSystemMetrics(SM_CYSCREEN);

    space.unit = 30;
    space.centre = {screenWidth / 2, screenHeigth / 2};
    space.dim = screenHeigth / 2 - 100;
    space.pixel = (long double)1 / space.unit;
    space.maxy = (long double)space.dim / space.unit;
    initwindow(screenWidth, screenHeigth, "", -3, -3);

    int spaceBorderX = space.centre.x + space.dim + 20;
    int spaceBorderY = space.centre.y + space.dim - 100;

    // draws plus and minus buttons
    button plusButton(spaceBorderX, spaceBorderY, spaceBorderX + 50, spaceBorderY + 50, "", drawPlus);
    button minusButton(spaceBorderX + 100, spaceBorderY, spaceBorderX + 150, spaceBorderY + 50, "", drawMinus);
    button exitButton(spaceBorderX + 200, spaceBorderY, spaceBorderX + 300, spaceBorderY + 100,
                      translationForLanguages["Exit"][currentLanguage], bar);

    button leftButton(spaceBorderX, spaceBorderY - 110, spaceBorderX + 100, spaceBorderY - 10,
                      translationForLanguages["Left"][currentLanguage], bar);
    button rightButton(spaceBorderX + 110, spaceBorderY - 110, spaceBorderX + 210, spaceBorderY - 10,
                       translationForLanguages["Right"][currentLanguage], bar);
    button recentreButton(spaceBorderX + 220, spaceBorderY - 110, spaceBorderX + 320, spaceBorderY - 10,
                          translationForLanguages["Center"][currentLanguage], bar);

    reEvaluateFunction(s, space);
    vector<button> functionButtons = {plusButton, minusButton, exitButton, leftButton, rightButton, recentreButton};
    drawButtons(functionButtons);
    while (true)
    {
        if (exitButton.isPressed())
            return;

        if (plusButton.isPressed() && space.translation_x == 0 && space.translation_y == 0)
        {
            space.unit += 10;
            space.pixel = (long double)1 / space.unit;
            space.maxy = (long double)space.dim / space.unit;
            reEvaluateFunction(s, space);
            drawButtons(functionButtons);
        }

        else if (minusButton.isPressed() && space.unit > 20 && space.translation_x == 0 && space.translation_y == 0)
        {
            space.unit -= 10;
            space.pixel = (long double)1 / space.unit;
            space.maxy = (long double)space.dim / space.unit;
            reEvaluateFunction(s, space);
            drawButtons(functionButtons);
        }
        else if (leftButton.isPressed())
        {
            space.translation_x -= 10;
            reEvaluateFunction(s, space);
            drawButtons(functionButtons);
        }
        else if (rightButton.isPressed())
        {
            space.translation_x += 10;
            reEvaluateFunction(s, space);
            drawButtons(functionButtons);
        }
        else if (recentreButton.isPressed())
        {
            space.translation_x = 0;
            reEvaluateFunction(s, space);
            drawButtons(functionButtons);
        }
    }
}
int main()
{
    initialize();
    mainMenu();

    return 0;
}
