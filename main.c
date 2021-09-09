#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
/*
* Item: An item that is being pushed to or popped from the stack
* It may be float (to be used while evaluating the postfix)
* It may be char (to be used while converting infix to postfix)
*/
///MOHAMED FARID ABDELAZIZ SALAMA 6905
///ADHAM MOHAMED ALY ABDELHAMID 6744
typedef union
{
    float fData;
    char  cData;
} Item;
/*
*
*/
typedef struct
{
    Item item[256];
    int top;
} Stack;
/*
*
*/
Stack* initialize()
{
    Stack *s = malloc(sizeof(Stack));
    s->top=-1;
    return s;
}
/*
*
*/
int isEmpty(Stack *s)
{
    return s->top == -1;
}
/*
*
*/
Item top(Stack *s)
{
    return s->item[s->top];
}
/*
*
*/
Item pop(Stack *s)
{
    return s->item[s->top--];
}
/*
*
*/
void push(Stack *s, float val)
{
    s->item[++s->top].fData = val;
}
/*
*
*/
void pushChar(Stack *s, char c)
{
    s->item[++s->top].cData = c;
}
/*
*
*/
int isFull(Stack *s)
{
    return s->top==255;
}
/*
*
*/
int priority(char c)
{
    if (c=='+' || c=='-') return 1;
    else if (c=='*' || c=='/') return 2;
    else if (c=='^') return 3 ;
    return 0;
}
/*
*
*/
int isOperator(char c)
{
    if( c=='(' || c=='+' || c=='-' || c=='/' || c=='*' || c=='^') return 1;
    else return 0;
}
/*
*
*/
int negativeInteger(char *infix,char c,int ptr)
{
    if (ptr == 0 && c == '-' ) return 1;
    else  if((isOperator(infix[ptr-1]) && c == '-' ) )return 1;
    else return 0;
}
/*
*
*/
int isAfter(Stack *s)
{
    if(s->item[s->top].cData == '(' ) return 1;
    else return 0;
}
/*
*
*/

char * removespaces(char * infix)
{

    char* x=malloc(266*sizeof(char));
    int i,j;
    for(i=0,j=0; infix[i]!='\0'; i++,j++)
    {

        while(infix[j]==' ')
            j++;
        x[i]=infix[j];
    }
    x[i]='\0';
    return x;
}
char *infixToPostfix(char *infix,char *postfix)
{
    char oneSpace[] = " ", tempInfix[256];
    int  ptr = 0;
    char *temp;
    Stack *s = initialize();

    while(infix[ptr] != '\0' )
    {

        if(isOperator(infix[ptr]) && !negativeInteger(infix,infix[ptr],ptr))
        {
            if(infix[ptr] == '(' )
            {
                pushChar(s,infix[ptr]); //(1+
                ptr++;
            }
            else if(isAfter(s))
            {
                pushChar(s,infix[ptr]);
                ptr++;
            }
            else if(priority(infix[ptr]) > priority(top(s).cData) || isEmpty(s))
            {
                pushChar(s,infix[ptr]);
                ptr++;
            }
            else if(priority(infix[ptr])==priority(top(s).cData))
            {
                char tempchar=pop(s).cData;
                strncat(postfix,&tempchar,1);
                pushChar(s,infix[ptr]);
                ptr++;
                strcat(postfix,oneSpace);
            }
            else if(priority(infix[ptr]) <priority(top(s).cData) && top(s).cData != '(' && top(s).cData != ')')
            {
                while(1)
                {
                    char tempChar;
                    if( isEmpty(s) ) break;
                    if( top(s).cData == '(' ) break;
                    tempChar = pop(s).cData;
                    strncat(postfix, &tempChar, 1);
                    strcat(postfix, oneSpace);
                }
            }
        }
        else if(infix[ptr]==')')
        {
            while(top(s).cData!='(')
            {
                char tempChar2;
                tempChar2 = pop(s).cData;
                strncat(postfix, &tempChar2, 1);
                strcat(postfix, oneSpace);
            }
            pop(s);
            ptr++;
        }
        else
        {
            if(negativeInteger(infix,infix[ptr],ptr))
            {
                char temp2[256] = "";
                char tempChar3 = '-';
                strncat(temp2, &tempChar3, 1);
                strcpy(tempInfix, infix);
                temp = strtok(tempInfix + ptr, " +-)(*/^");
                ptr += strlen(temp) + 1;
                strcat(temp2, temp);
                strcat(postfix, temp2);
                strcat(postfix, oneSpace);
            }
            else
            {
                strcpy(tempInfix,infix);
                temp = strtok(tempInfix + ptr, " +-)(*/^");
                ptr+=strlen(temp);
                strcat(postfix, temp);
                strcat(postfix, oneSpace);
            }
        }
    }
    while(!isEmpty(s))
    {
        char tempChar6 = pop(s).cData;
        strncat(postfix, &tempChar6, 1);
        strcat(postfix,oneSpace);
    }
    return postfix;
}
/*
*
*/
void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}
/*
*
*/
/// This function checks if a given token is a number ( positive or negative )
int isNumber(char *token)
{
    return isdigit(*token) || ( *token == '-' && isdigit(token[1]) );
}
/*
*
*/
/// This function evaluates a postfix expression and returns the result
float evaluatePostfix(char postFix[])
{
    float a, b;
    Stack *stack = initialize();
    char *token = strtok(postFix," ");

    while(token != NULL)
    {
        // The following condition checks if the token is a number. If so, it is converted to float and pushed into the stack
        if(isNumber(token))
        {
            push(stack, atof(token));
        }
        // The following condition checks if the token is an operator. If so, the top two elements are popped to undergo the respective operation
        // The result is then pushed back into the stack
        else if(isOperator(*token))
        {
            a = pop(stack).fData;
            b = pop(stack).fData;
            switch(*token)
            {
            case '+':
                push(stack, b + a );
                break;
            case '-':
                push(stack, b - a );
                break;
            case '*':
                push(stack, b * a );
                break;
            case '/':
                push(stack, b / a );
                break;
            case '^':
                push(stack, pow(b, a) );
                break;
            default:
                break;
            }
        }
        token = strtok(NULL, " "); //Proceed with tokenization
    }
    return pop(stack).fData;
}
/*
*
*/
int main()
{
    char infixExpr[256];
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit:\nYou can write the expression with or without spaces or a mix:  ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {


        char postfixExpr[256] = "";
        char*x;
        replaceNewLineBySpace(infixExpr);
        x=removespaces(infixExpr);
        printf("Postfix : %s\n",infixToPostfix(x, postfixExpr));
        float result = evaluatePostfix(postfixExpr);
        printf("Result: %f\n\n", result);
       printf("Enter an expression you want to evaluate or Ctrl+Z to exit :\nYou can write the expression with or without spaces or a mix:  ");
    }
    return 0;
}
