
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 10

typedef struct {
    int arr[MAX_STACK_SIZE];

    int top;  // 栈顶指针

} Stack;


// 初始化,栈
void init_stack(Stack **stack) {

    (*stack) = (Stack *)malloc(sizeof(Stack) );
    
    // 初始化为,0
    memset((*stack)->arr, 0, MAX_STACK_SIZE * sizeof(int));

    (*stack)->top = -1;

}

void destroy_stack(Stack *stack) {
    free(stack);
}

// 是否,满
int is_full_stack(Stack *stack) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        return 1;
    } else {
        return 0;
    }
}

// 是否,空
int is_empty_stack(Stack *stack) {
    if (stack->top == - 1) {
        return 1;
    } else {
        return 0;
    }
}

// 入栈
int push_stack(Stack *stack, int value) {
    // printf("AA %d\n", stack->top);

    if (is_full_stack(stack)) { // 满的
        return 0;
    } else {
        ++stack->top;
        stack->arr[stack->top] = value;
    }
    // printf("AA %d\n", stack->top);
    return 1; 
}

// 出栈
int pop_stack(Stack *stack, int *value) {
    if (is_empty_stack(stack)) {
        return 0;
    } else {
        *value = stack->arr[stack->top];
        stack->top--;

        return -1;
    }
}

// 获取,栈顶值
int get_top_value(Stack *stack, int *value) {
    if (is_empty_stack(stack)) {
        return 0;
    } else {
        *value = stack->arr[stack->top];
    }
}

// 获取,栈中的元素
int get_stack_num(Stack *stack) {
    return stack->top + 1;
}

// int main() {
    
//     Stack *stack;

//     init_stack(&stack);

//     push_stack(stack, 11);
//     push_stack(stack, 22);
//     push_stack(stack, 33);

//     int num =get_stack_num(stack );
//     printf("%d\n", num);

//     int value;
//     pop_stack(stack, &value);
//     printf("%d\n", value);

//     destroy_stack(stack);

// }
