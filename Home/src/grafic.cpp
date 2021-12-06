#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;
// g++ -o grafic grafic.cpp -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
inline void setBackground()
{
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    readimagefile("ImageBackground.jpg", 0, 0, screenWidth, screenHeigth);
}
class button
{
public:
    int x1, y1;
    int x2, y2;
    string txt;
    button(int xA, int yA, int xB, int yB, const string &text)
    {
        txt = text;
        x1 = xA;
        x2 = xB;
        y1 = yA;
        y2 = yB;
        setfillstyle(SLASH_FILL, YELLOW);
        bar(x1, y1, x2, y2);

        settextstyle(3, HORIZ_DIR, 3);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy((x1 + x2) / 2, (y1 + y2) / 2, (char *)txt.c_str());
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
void mainMenu();
bool isValid(char c)
{
    if (isalnum(c) || c == ' ' || c == '*' || c == '+' || c == '-' || c == '^')
        return true;
    return false;
}
void updateText(int &x, int y, string &eq, char c)
{
    if (!isValid(c))
        ; // inform them somehow
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
    }
}
inline void setBlackboard()
{
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    readimagefile("Blackboard.jpg", 0, 0, screenWidth, screenHeigth);
}
void drawFunction(string s);
inline void functionInput()
{
    cleardevice();
    setBlackboard();
    string equation;
    outtextxy(400, 300, "Please write your equation here");

    int crtX = 425, crtY = 425;
    while (1)
    {
        while (!kbhit())
            ;
        char c = getch();
        if (c != 8 && c != 13)
            updateText(crtX, crtY, equation, c);
        else if (c == 8)
            backSpace(crtX, crtY, equation);
        else
        {
            cerr << equation.size();
            cerr << equation;
            drawFunction(equation);
            break;
        };
    }
    cout << equation;
    mainMenu();
}
inline void initialize()
{
    DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenWidth, screenHeigth, "", -3, -3);
    //setwritemode(XOR_PUT); doesn't work with text
}

void mainMenu()
{
    cleardevice();
    setBackground();
    button Start(200, 100, 400, 200, "Start");
    button Languages(200, 200, 400, 300, "Limbi");
    button Exit(200, 300, 400, 400, "Iesire");
    while (1)
    {
        if (Start.isPressed())
        {
            functionInput();
        }
        else if (Languages.isPressed())
        {
            circle(400, 400, 13);
        }
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
    long double pixel, maxx, maxy;

} space;

void draw_space(myspace space)
{
    setcolor(BLACK);
    rectangle(space.centre.x - space.dim, space.centre.y - space.dim, space.centre.x + space.dim, space.centre.y + space.dim);

    setcolor(WHITE);
    line(space.centre.x - space.dim, space.centre.y, space.centre.x + space.dim, space.centre.y);
    line(space.centre.x, space.centre.y - space.dim, space.centre.x, space.centre.y + space.dim);
}

long double pixelvalue(int x, myspace space)
{
    return (x - space.centre.x) * space.pixel;
}

int sign(long double value)
{
    if (value < 0)
        return 1;
    else
        return -1;
}

int normalizare(long double value)
{
    if (fabs(value) > space.maxy)
        return space.centre.y + sign(value) * space.dim;

    return space.centre.y + sign(value) * fabs(value) * space.unit;
}

int priority(int op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return -1;
}

void process_op(stack<long double> &st, char op)
{
    long double r, l;
    r = st.top();
    st.pop();
    if (op != 's' && op != 'c' && op != 't')
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
        st.push(l * r);
        break;
    case '/':
        st.push(l / r);
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
    }
}

long double evaluate(string &s, long double x)
{
    stack<long double> st;
    stack<char> op;
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (s[i] == ' ')
            continue;

        if (s[i] == 'x')
        {
            st.push(x);
            continue;
        }

        if (s[i] == '(' || s[i] == 's' || s[i] == 'c' || s[i] == 't')
        {
            op.push(s[i]);
            if (s[i] == 's')
                i += 2;
            else if (s[i] == 'c')
                i += 2;
            else if (s[i] == 't')
                i++;
        }

        else if (s[i] == ')')
        {
            while (op.top() != '(')
            {
                process_op(st, op.top());
                op.pop();
            }
            op.pop();

            if (!op.empty() && (op.top() == 's' || op.top() == 'c' || op.top() == 't'))
            {
                process_op(st, op.top());
                op.pop();
            }
        }

        else if ('0' <= s[i] && s[i] <= '9')
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
            st.push(number);
        }

        else
        {
            while (!op.empty() && priority(op.top()) >= priority(s[i]))
            {
                process_op(st, op.top());
                op.pop();
            }
            op.push(s[i]);
        }
    }

    while (!op.empty())
    {
        process_op(st, op.top());
        op.pop();
    }
    return st.top();
}
void drawFunction(string s)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeigth = GetSystemMetrics(SM_CYSCREEN);
    space.unit = 30;
    space.centre = {screenWidth/2, screenHeigth/2};
    space.dim = max(screenWidth, screenWidth);
    space.pixel = (long double)1 / space.unit;
    space.maxy = (long double)space.dim / space.unit;
    initwindow(screenWidth, screenHeigth, "", -3, -3);
    for (int punct = space.centre.x - space.dim; punct < space.centre.x + space.dim; punct++)
    {
        int y1 = normalizare(evaluate(s, pixelvalue(punct, space)));
        int y2 = normalizare(evaluate(s, pixelvalue(punct + 1, space)));

        setcolor(RED);
        line(punct, y1, punct + 1, y2);
    }

    draw_space(space);
    getch();
}
int main()
{
    initialize();
    mainMenu();

    return 0;
}
