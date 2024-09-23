#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void run_wc_test(char *test_description, char *wc_args) {
    int pid = fork();
    if (pid == 0) {
        char *args[4];
        args[0] = "wc";
        args[1] = wc_args;
        args[2] = "count_sample.txt";
        args[3] = 0;

        printf("\n%s\n", test_description);
        exec("wc", args);
        exit(0);
    } else {
        wait(0);
    }
}

int main(void) {
    run_wc_test("Test with no flags", "");
    run_wc_test("Test counting punctuation (-p)", "-p");
    run_wc_test("Test counting digits (-d)", "-d");
    run_wc_test("Test counting uppercase letters (-u)", "-u");
    run_wc_test("Test counting lowercase letters (-l)", "-l");
    run_wc_test("Test with invalid option (-z)", "-z");

    exit(0);
}
