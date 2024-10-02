#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <string.h>

enum type{INT, CHAR};
struct node{
    enum type t;
    union{
        int i;
        char c;
    }data;
    struct node *next;
};

struct stack{
    struct node *top;
    int size;
};

void initializeStack(struct stack* pStack){
    pStack->top = NULL;
    pStack->size = 0;
}

int isEmpty (struct stack* pStack){
    return pStack->top == NULL;
}

int size (struct stack* pStack){
    return pStack->size;
}

void top (struct stack* pStack, int* i, char* c, enum type t){
    if (isEmpty(pStack))
        return ;
    if(t == INT)
        *i = pStack->top->data.i;
    else if(t == CHAR)
        *c = pStack->top->data.c;
    return ;
}

void push (struct stack* pStack ,int i, char c, enum type t){
    struct node* pNewNode = malloc(sizeof(struct node));
    pNewNode->t = t;
    if(t == INT)
        pNewNode->data.i = i;
    else if(t == CHAR)
        pNewNode->data.c = c;
    pNewNode->next = pStack->top;
    pStack->top = pNewNode;
    pStack->size++;
}

int pop (struct stack* pStack ,int* i, char* c, enum type* t){
    if(isEmpty(pStack))
        return 0;

    struct node* current;

    if(pStack->top->t == INT)
        *i = pStack->top->data.i;
    else if(pStack->top->t == CHAR)
        *c = pStack->top->data.c;

    *t = pStack->top->t;
    current = pStack->top;
    pStack->top = current->next;
    free(current);
    pStack->size--;
    return 1;
}

void printStack(struct stack* pStack){
    struct node* current;
    current = pStack->top;
    while(current != NULL){
        if(current->t == INT)
            printf("%d",current->data.i);
        else
            printf("%c",current->data.c);
        current = current->next;
    }
    printf("\n");
}

int IsOperand(char c){
    if (c == 'a' || c == 'b' || c == 'c')
        return 1;
    return 0;
}

int IsOperator(char c){
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
        return 1;
    return 0;
}

int OperatorWeight(char op){
    int weight = -1;
    if(op == '+' || op == '-')
        weight = 1;
    else if(op == '*' || op == '/')
        weight = 2;
    else if(op == '^')
        weight = 3;
    return weight;
}

int HigherPrecedence(char op1, char op2){
    int op1Weight = OperatorWeight(op1);
    int op2Weight = OperatorWeight(op2);
    if (op1Weight == op2Weight){
        if (op1 == '^')
            return 0;
        else
            return 1;
    }
    return op1Weight > op2Weight;
}

void num (char stg[20], int* x){
    int i = 0;
    int num = 0, sign = 1;
    while (stg[i] != '\0'){
        if (stg[i] == ' '||stg[i] == 'a'||stg[i] == 'b'||stg[i] == 'c'||stg[i] == '='){
            i++;
            continue;
        }
        if (stg[i] == '-')
            sign = -1;
        else
            num = num * 10 + (stg[i] - '0');
        i++;
    }
    *x = sign * num;
}

int operation(int op1, int op2, char op){
    int ans;
    switch (op) {
    case '+':
        ans = op2 + op1;
        break;
    case '-':
        ans = op2 - op1;
        break;
    case '*':
        ans = op2 * op1;
        break;
    case '/':
        ans = op2 / op1;
        break;
    case '^':
        ans = pow(op2,op1);
        break;
    default:
        ans = 0;
    }
    return ans;
}

void CheckMinuses (char stg[20], char stgnew[20]){
    int i, j;
    i = j = 0;
    while (stg[i] != '\0'){
        if (stg[i] == ' '){
            i++;
            continue;
        }
        if (stg[i] == '-' && stg[i+1] == '-'){
            if (i == 0 || stg[i-1] == '(' || IsOperator(stg[i-1]))
                i+=2;
            else if (IsOperand(stg[i-1]) || stg[i-1] == ')'){
                stgnew[j] = '+';
                j++;
                i+=2;
            }
        }
        else{
            stgnew[j] = stg[i];
            j++;
            i++;
        }
    }
    stgnew[j] = '\0';
}
int CheckParentheses (char stg[20]){
    int i, count;
    i = count = 0;
    while (stg[i] != '\0'){
        if (stg[i] == '('){
            if(count < 0)
                return 0;
            count++;
        }
        else if (stg[i] == ')')
            count--;
        i++;
    }
    if(count == 0)
        return 1;
    else
        return 0;
}

int ExpressionIsValid (char stg[20]){
    int i = 0;
    int Noperator , Noperand;
    Noperator = Noperand = 0;
    if (!CheckParentheses(stg))
        return 0;
    else{
        while (stg[i] != '\0'){
            if (i == 0 && stg[i] == '-'){
                if (stg[i+1] == ')')
                    return 0;
            }
            else if(stg[i] == '('){
                if ((i && IsOperand(stg[i-1])) || (stg[i+1] && IsOperator(stg[i+1])))
                     return 0;
            }
            else if(stg[i] == ')'){
                if ((i &&IsOperator(stg[i-1])) || (stg[i+1] && IsOperand(stg[i+1])))
                     return 0;
            }
            else if (IsOperand(stg[i])){
                if (stg[i+1] && (IsOperand(stg[i+1]) || stg[i+1] == '('))
                    return 0;
                Noperand++;
            }
            else if (IsOperator(stg[i])){
                if ((stg[i+1] && IsOperator(stg[i+1])) || (stg[i+1] && stg[i+1] == ')'))
                   return 0;
                Noperator++;
            }
            i++;
        }
        if(Noperand == (Noperator + 1))
            return 1;
        else
            return 0;
    }
}

void StgToStack (char stg[20], struct stack* infix){
    int i = 0;
    char c;
    enum type t;
    struct stack temp;
    initializeStack(&temp);
    while (stg[i] != '\0'){
        push(&temp, 0, stg[i], CHAR);
        i++;
    }
    while(!(isEmpty(&temp))){
        pop(&temp, &i, &c, &t);
        push(infix, i, c, t);
    }
}

void InfixToPostfix(struct stack* infix, struct stack* postfix){
    int i;
    char c, op;
    enum type t;
    struct stack prefix, operators;
    initializeStack(&prefix);
    initializeStack(&operators);
    while(!(isEmpty(infix))){
        pop(infix, &i, &c, &t);
        if(IsOperator(c)){
            while(!isEmpty(&operators) && operators.top->data.c != '('&& HigherPrecedence(operators.top->data.c, c)){
                pop(&operators, &i, &op, &t);
                push(&prefix, 0, op, CHAR);
            }
            push(&operators, 0, c, CHAR);
        }
        else if(IsOperand(c))
            push(&prefix, 0, c, CHAR);
        else if(c == '(')
            push(&operators, 0, c, CHAR);
        else if(c == ')'){
            while (!isEmpty(&operators) && operators.top->data.c != '(') {
                pop(&operators, &i, &op, &t);
                push(&prefix, 0, op, CHAR);
            }
            pop(&operators, &i, &op, &t);
        }
    }
    while (!isEmpty(&operators)) {
        pop(&operators, &i, &op, &t);
        push(&prefix, 0, op, CHAR);
    }
    while(!(isEmpty(&prefix))){
        pop(&prefix, &i, &c, &t);
        push(postfix, 0, c, CHAR);
    }
}

void eval(struct stack* postfix, int A, int B, int C, int* ans){
    int i;
    char x;
    enum type t;
    int op1, op2;
    struct stack operands;
    initializeStack(&operands);
    while(!isEmpty(postfix)){
        pop(postfix, &i, &x, &t);
        if(IsOperator(x)){
            pop(&operands, &op1, &x, &t);
            if(!isEmpty(&operands)){
                pop(&operands, &op2, &x ,&t);
                push(&operands, operation(op1,op2,x), ' ',INT);
            }
            else
                push(&operands, -op1, ' ',INT);
        }
        else if(IsOperand(x)){
            if(x == 'a')
                push(&operands, A, ' ', INT);
            else if(x == 'b')
                push(&operands, B, ' ', INT);
            else if(x == 'c')
                push(&operands, C, ' ', INT);
        }
    }
    *ans = operands.top->data.i;
}

int main(){
    struct stack infix;
    struct stack postfix;
    char stg[20], stgnew[20];
    int A, B, C;
    int ans;
    scanf("%s",stg);
    CheckMinuses(stg, stgnew);
    if(!ExpressionIsValid(stgnew))
        printf("Error");
    else{
        initializeStack(&infix);
        StgToStack(stgnew,&infix);
        initializeStack(&postfix);
        InfixToPostfix(&infix,&postfix);
        scanf("%s",stg);
        num(stg ,&A);
        scanf("%s",stg);
        num(stg ,&B);
        scanf("%s",stg);
        num(stg ,&C);
        printStack(&postfix);
        eval(&postfix, A, B, C, &ans);
        printf("%d",ans);
    }
    return 0;
}
