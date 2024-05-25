#include <stdio.h>

#include "stack_c.c"

typedef struct {
    Stack *numStack; // 数字栈
    Stack *symStack; // 符号栈

    char *str;

} CalManager;


// 单个值
typedef struct {
    int pos; // 索引位置
    int flag ; // 0:数字  1:符号
    int value;
} ItemValue;


// 初始化
void init_calculate(CalManager *calManager, char *str) {
    init_stack(&calManager->numStack);
    init_stack(&calManager->symStack);

    calManager->str = str;

    push_stack(calManager->symStack, -1); // 将-1, 入栈. 作为最底层的
}

int operate_sym(int sym, int value1, int value2) {
    int ret;
    switch (sym) {
        case '+':
            ret = value1 + value2;
            break;

        case '-':
            ret = value1 - value2;
            break;
    
        case '*':
            ret = value1 * value2;
            break;
    
        case '/':
            ret = value1 / value2;
            break;
    }
    return ret;
}

// 获取,一个数字,或者,符号
int get_value(CalManager *calManager, ItemValue *itemValue) {
    char *arr = calManager->str;
    
    int pos = itemValue->pos;

    int sum = 0; // 返回的值

    int valueFlag = 0;

    if (arr[pos] >= '0' && arr[pos] <='9' ) {
        // 这是,数字
        while (arr[pos] >= '0' && arr[pos] <='9' ) {
            sum  = sum * 10 + arr[pos] - '0';
            pos++;
        }
    } else {
        valueFlag = 1; // 符号
        sum = arr[pos];
        pos++;
    }
    // 修改,移动的值
    itemValue->pos = pos;
    itemValue->flag = valueFlag;
    itemValue->value = sum;

}


// 删除
void destory_calculate(CalManager *calManager) {
    destroy_stack(calManager->numStack);
    destroy_stack(calManager->symStack);

}


// 获取,符号的优先级
int get_sym_priv(int sym) {
    int priv = 0;
    switch (sym) {
        case -1: // 最底层的
            priv = -1;
            break;
        
        // ( 的优先级,是不看的
        case '(':  case ')': // 优先级,较低. 遇到这个,一般都要出栈 
            priv = 0;
        break;

        case '+': case '-':
            priv = 1;
        break;

        case '*': case '/':
            priv = 2;
        break;
    }
    return priv;
}

void calculate(CalManager *calManager, char *str) {
    init_calculate(calManager, str);

    ItemValue itemValue;

    int pos = 0;
    int valueFlag = 0;
    int value = 0;

    int sym; // 当前符号
    int symPriv; // 优先级

    int lastSym = 0; //上一个符号

    int lastSymPriv; // 上一个符号的优先级

    int len = strlen(calManager->str);

    while (pos < len) {
        itemValue.pos = pos;
        get_value(calManager, &itemValue);

        // 获取,值
        valueFlag = itemValue.flag;
        value = itemValue.value;
        pos = itemValue.pos; // 下一个为止

        // if (valueFlag == 0) { // 入栈
        //     printf("数字:  %d\n", value);
        // } else {
        //     printf("符号:  %c\n", value);
        // }
        
        // 如果,当前值,是数字. 那么,入栈. 
        if (valueFlag == 0) { // 数字, 入栈
            push_stack(calManager->numStack, value);
            // printf("value: %d\n", value);

        } else {
            // 如果是,符号. 则有可能入栈, 也有可能出栈
            sym = value; // 当前的符号

            symPriv = get_sym_priv(sym); // 当前符号的优先级

            // 当前的符号,优先级高. 就入栈
            // 当前的符号,优先级低. 那么, 就需要,将之前的符号,先处理掉. 将结果入栈
                // 直到,当前符号的优先级,较高为止

            while (1) {
  
                // 下一次,循环
                // 获取,上一个符号
                get_top_value(calManager->symStack, &lastSym);

                // 上一个符号的优先级
                lastSymPriv = get_sym_priv(lastSym);

                // printf("debug: %c %d  %c %d\n", lastSym, lastSymPriv, sym, symPriv);

                // 如果,当前符号的优先级高, 或者, 是(, 或者是,(). 那么,都不出栈
                if (lastSym == '(' && sym == ')') {
                    pop_stack(calManager->symStack, &lastSym); // (,出栈

                    break;
                } else if (symPriv >= lastSymPriv || sym == '('  ) { // 
                    // 当前符号的优先级,比较高
                    break;
                } else {
                    // printf("AAA\n");
                    // 将上一个符号,先进行运算
                    pop_stack(calManager->symStack, &lastSym); // 将上一个符号,出栈
                    // 上一个值

                    // 取出,数字栈中的值
                    int value1;
                    int value2;

                    pop_stack(calManager->numStack, &value2);
                    pop_stack(calManager->numStack, &value1);

                    value1 = operate_sym(lastSym, value1, value2 );
                    
                    // 将值, 入栈
                    push_stack(calManager->numStack, value1);

                    // printf("PUSH %d\n", value1);

                }
            } // end of while(1)

            // 只有 (, 不入栈
            if (sym != ')' ) {
                // 最后,肯定会将当前符号,入栈的
                push_stack(calManager->symStack, sym);
                // printf("push symal %c\n", sym);
            }
        
        }
    } // end of while

    // 最后,将所有的符号,出栈
    while (get_stack_num(calManager->symStack) > 1) {
        // printf("I enter\n");
        int value1;
        int value2;

        pop_stack(calManager->symStack, &sym);

        pop_stack(calManager->numStack, &value2);
        pop_stack(calManager->numStack, &value1);

        value1 = operate_sym(sym, value1, value2);
        
        // 入栈
        push_stack(calManager->numStack, value1);
    }

    // 取出值
    pop_stack(calManager->numStack, &value);

    printf("result: %d\n", value);

    destory_calculate(calManager);
}


int main() {
    CalManager calManager;

    // char *str = "10";
    // char *str = "1+23";  // 24
    // char *str = "1+6/3+5*4-9"; // 14

    // char *str = "(103)";
    char *str = "13+(103*3+3)"; // 312+13 = 325


    calculate(&calManager, str);

}