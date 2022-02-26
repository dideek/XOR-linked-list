#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define MAX_LENGTH 20 // Max dlugosc wydawanej komendy
using namespace std;
struct Elem {
	int key;
	Elem* xptr;
};
Elem* first, * last, * prev, * next, * actual;
enum ecmd {
	CMD_ADD_BEG,
	CMD_PRINT_FORWARD,
	CMD_PRINT_BACKWARD,
	CMD_ADD_END,
	CMD_ACTUAL,
	CMD_ADD_ACT,
	CMD_NEXT,
	CMD_PREV,
	CMD_DEL_BEG,
	CMD_DEL_END,
	CMD_DEL_VAL,
	CMD_DEL_ACT,
	CMD_ERROR
};

int parseEnum(char* cmd) {
	return !strcmp("ADD_BEG", cmd) ? CMD_ADD_BEG :
		!strcmp("ADD_END", cmd) ? CMD_ADD_END :
		!strcmp("ADD_ACT", cmd) ? CMD_ADD_ACT :
		!strcmp("PRINT_FORWARD", cmd) ? CMD_PRINT_FORWARD :
		!strcmp("PRINT_BACKWARD", cmd) ? CMD_PRINT_BACKWARD :
		!strcmp("ACTUAL", cmd) ? CMD_ACTUAL :
		!strcmp("NEXT", cmd) ? CMD_NEXT :
		!strcmp("PREV", cmd) ? CMD_PREV :
		!strcmp("DEL_BEG", cmd) ? CMD_DEL_BEG :
		!strcmp("DEL_END", cmd) ? CMD_DEL_END :
		!strcmp("DEL_VAL", cmd) ? CMD_DEL_VAL :
		!strcmp("DEL_ACT", cmd) ? CMD_DEL_ACT :
		CMD_ERROR;
}
Elem* XOR(Elem* a, Elem* b) {
	return (Elem*)((uintptr_t)(a) ^ (uintptr_t)(b));
}
void add_beg(int n) {
	Elem* tmp = (Elem*)malloc(sizeof(Elem));
	tmp->key = n;
	if (first)
	{
		tmp->xptr = first;
		if (first == actual) {
			prev = tmp;
		}
		first->xptr = XOR(tmp, first->xptr);

	}
	else
	{
		tmp->xptr = NULL;
		last = tmp;
		actual = tmp;
		prev = tmp;
		next = tmp;
	}
	first = tmp;
}
void add_end(int n) {
	Elem* tmp = (Elem*)malloc(sizeof(Elem));
	tmp->key = n;
	if (last) {
		tmp->xptr = last;
		if (last == actual) {
			next = tmp;
		}
		last->xptr = XOR(tmp, last->xptr);
	}
	else
	{
		tmp->xptr = NULL;
		first = tmp;
		actual = tmp;
		prev = tmp;
		next = tmp;
	}
	last = tmp;
}
void print_forward() {
	Elem* tmp = first;
	Elem* prev_tmp = NULL;
	Elem* next_tmp = NULL;
	if (!first) printf("NULL");
	while (tmp != NULL) {
		printf("%d ", tmp->key);
		next_tmp = XOR(prev_tmp, tmp->xptr);
		prev_tmp = tmp;
		tmp = next_tmp;
	}
	printf("\n");
}
void print_backward() {
	Elem* tmp = last;
	Elem* prev_tmp = NULL;
	Elem* next_tmp = NULL;
	if (!first) printf("NULL");
	while (tmp != NULL) {
		printf("%d ", tmp->key);
		prev_tmp = XOR(tmp->xptr, next_tmp);
		next_tmp = tmp;
		tmp = prev_tmp;
	}
	printf("\n");
}
void print_actual() {
	if (!first) printf("NULL\n");
	else printf("%d\n", actual->key);
}
void print_next() {
	if (!actual) printf("NULL\n");
	else if (actual == last) {
		actual = first;
		next = actual->xptr;
		prev = last;
		printf("%d\n", actual->key);
	}
	else if (actual == first) {
		prev = actual;
		actual = first->xptr;
		printf("%d\n", actual->key);
		next = XOR(first, actual->xptr);
	}
	else {
		Elem* tmp = actual;
		actual = XOR(prev, actual->xptr);
		prev = tmp;
		next = XOR(actual->xptr, prev);
		printf("%d\n", actual->key);
	}
}
void print_prev() {
	if (!actual) printf("NULL\n");
	else if (actual == first) {
		actual = last;
		prev = last->xptr;
		next = first;
		printf("%d\n", actual->key);
	}
	else if (actual == last) {
		actual = last->xptr;
		next = last;
		prev = XOR(next, actual->xptr);
		printf("%d\n", actual->key);
	}
	else {
		Elem* tmp = actual;
		actual = XOR(next, actual->xptr);
		next = tmp;
		prev = XOR(next, actual->xptr);
		printf("%d\n", actual->key);
	}
}
void add_act(int n) {
	Elem* tmp = (Elem*)malloc(sizeof(Elem));
	tmp->key = n;
	if (first == actual) {
		actual->xptr = XOR(tmp, next);
		tmp->xptr = actual;
		first = tmp;
		prev = tmp;
	}
	else {
		tmp->xptr = XOR(prev, actual);
		prev->xptr = XOR(prev->xptr, actual);
		prev->xptr = XOR(prev->xptr, tmp);      // Prev teraz pokazuje na swój poprzednik i na tmp
		actual->xptr = XOR(actual->xptr, prev);
		actual->xptr = XOR(actual->xptr, tmp); // Actual teraz pokazuje na tmp i swój następnik
		prev = tmp;
	}
}
void del_beg() {
	Elem* tmp;
	if (first == last)
	{
		free(first);
		first = nullptr;
		last = nullptr;
		actual = nullptr;
		prev = nullptr;
		next = nullptr;
	}
	else
	{

		if (first == actual) {
			actual = last;
			prev = last->xptr;
			next = first;
		}
		first->xptr->xptr = XOR(first->xptr->xptr, first);
		tmp = first->xptr;
		free(first);
		first = tmp;
	}
}
void del_end() {
	Elem* tmp;
	if (first == last)
	{
		free(first);
		first = nullptr;
		last = nullptr;
		actual = nullptr;
		prev = nullptr;
		next = nullptr;
	}
	else
	{
		if (last == actual) {
			actual = last->xptr;
			prev = last->xptr->xptr;
			next = first;
		}
		last->xptr->xptr = XOR(last->xptr->xptr, last);
		tmp = last->xptr;
		free(last);
		last = tmp;
	}
}
void del_act() {
	if (first == actual) {
		del_beg();
	}
	else if (actual == last) {
		del_end();
	}
	else {
		Elem* tmp = NULL;
		tmp = XOR(actual, next->xptr);
		next->xptr = XOR(prev, tmp); 
		tmp = XOR(prev->xptr, actual);
		if (prev == first) prev->xptr = next;
		else prev->xptr = XOR(tmp, actual); 
		free(actual);
		actual = prev;
		if (prev == first) prev = last;
		else prev = XOR(prev->xptr, actual);




	}
}
void del_val(int n) {
	Elem* tmp = first;
	Elem* prev_tmp = NULL;
	Elem* next_tmp = NULL;
	while (tmp != last) {
		if (tmp->key == n)
		{
			if (tmp == first) {
				if (prev == tmp) prev = last;
				if (actual == tmp) actual = last;
				next_tmp = XOR(nullptr, tmp->xptr);
				next_tmp->xptr = XOR(tmp, next_tmp->xptr);
				free(tmp);
				tmp = next_tmp;
				first = tmp;

			}
			else {
				next_tmp = XOR(prev_tmp, tmp->xptr);
				next_tmp->xptr = XOR(next_tmp->xptr, tmp);
				next_tmp->xptr = XOR(next_tmp->xptr, prev_tmp);
				prev_tmp->xptr = XOR(prev_tmp->xptr, tmp);
				prev_tmp->xptr = XOR(prev_tmp->xptr, next_tmp);
				if (tmp == prev) prev = prev_tmp;
				else if (tmp == actual) actual = prev_tmp;
				else if (tmp == next) next = next_tmp;
				free(tmp);
				tmp = next_tmp;
			}
		}
		else {
			next_tmp = XOR(prev_tmp, tmp->xptr);
			prev_tmp = tmp;
			tmp = next_tmp;
		}
	}
	if (tmp->key == n) {
		del_end();
	}
}
int main() {
	char cmd[MAX_LENGTH];
	int n;
	while (scanf("%s", &cmd) > 0)
	{
		switch (parseEnum(cmd))
		{
		case CMD_ADD_BEG:
			scanf("%d", &n);
			add_beg(n);
			break;
		case CMD_ADD_END:
			scanf("%d", &n);
			add_end(n);
			break;
		case CMD_ADD_ACT:
			scanf("%d", &n);
			add_act(n);
			break;
		case CMD_PRINT_FORWARD:
			print_forward();
			break;
		case CMD_PRINT_BACKWARD:
			print_backward();
			break;
		case CMD_ACTUAL:
			print_actual();
			break;
		case CMD_NEXT:
			print_next();
			break;
		case CMD_PREV:
			print_prev();
			break;
		case CMD_DEL_BEG:
			del_beg();
			break;
		case CMD_DEL_END:
			del_end();
			break;
		case CMD_DEL_VAL:
			scanf("%d", &n);
			del_val(n);
			break;
		case CMD_DEL_ACT:
			del_act();
			break;
		case CMD_ERROR:
		default:
			break;
		}

	}

	return 0;
}