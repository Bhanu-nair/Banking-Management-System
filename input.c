#include "banking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

static int equals_ignore_case(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

void input_line(const char *prompt, char *out, size_t size, int allow_empty) {
    char buffer[512];
    for (;;) {
        size_t len;
        printf("%s", prompt);
        if (!fgets(buffer, sizeof buffer, stdin)) { puts("\nInput ended. Saving data and closing."); (void)save_data(); free_all_customers(); exit(EXIT_SUCCESS); }
        len = strlen(buffer);
        if (len && buffer[len - 1] == '\n') buffer[--len] = '\0';
        else if (!feof(stdin)) { int ch; while ((ch = getchar()) != '\n' && ch != EOF) {} puts("Input is too long. Please try again."); continue; }
        while (len && isspace((unsigned char)buffer[len - 1])) buffer[--len] = '\0';
        size_t start = 0; while (buffer[start] && isspace((unsigned char)buffer[start])) start++;
        if (!allow_empty && buffer[start] == '\0') { puts("This field cannot be empty."); continue; }
        if (strlen(buffer + start) >= size) { printf("Maximum length is %zu characters.\n", size - 1); continue; }
        memmove(out, buffer + start, strlen(buffer + start) + 1);
        return;
    }
}

int input_int(const char *prompt, int min, int max) {
    char s[80], *end; long v;
    for (;;) {
        input_line(prompt, s, sizeof s, 0); errno = 0; v = strtol(s, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (errno == 0 && *end == '\0' && v >= min && v <= max) return (int)v;
        printf("Enter a whole number between %d and %d.\n", min, max);
    }
}

double input_money(const char *prompt) {
    char s[100], *end; double v;
    for (;;) {
        input_line(prompt, s, sizeof s, 0); errno = 0; v = strtod(s, &end);
        while (isspace((unsigned char)*end)) end++;
        if (errno == 0 && *end == '\0' && v > 0.0 && v <= MAX_AMOUNT) return v;
        printf("Enter a positive amount not exceeding %.2f.\n", MAX_AMOUNT);
    }
}

int input_yes_no(const char *prompt) {
    char s[16];
    for (;;) { input_line(prompt, s, sizeof s, 0); if (equals_ignore_case(s,"y") || equals_ignore_case(s,"yes")) return 1; if (equals_ignore_case(s,"n") || equals_ignore_case(s,"no")) return 0; puts("Please enter Y or N."); }
}

void pause_enter(void) { char s[8]; printf("\nPress Enter to continue..."); if (!fgets(s, sizeof s, stdin)) clearerr(stdin); }
void print_header(const char *title) { printf("\n============================================================\n  %s\n============================================================\n", title); }
void format_time(time_t value, char *out, size_t size) { struct tm *tmv = localtime(&value); if (!tmv || !strftime(out, size, "%Y-%m-%d %H:%M:%S", tmv)) snprintf(out, size, "Unknown"); }
